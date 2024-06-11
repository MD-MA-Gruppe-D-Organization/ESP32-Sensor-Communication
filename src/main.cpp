#include <Wifi.h>
#include <PubSubClient.h>
#include "wifi_credentials.h"
#include <Ultrasonic.h>
#include <Arduino.h>
#include <Wire.h> 
#include "SSD1306Wire.h" 

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-------------------------");
}

// Display
SSD1306Wire display(0x3c, 4, 15);

// WiFi
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;
WiFiClient espClient;

void wifi_setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
    display.clear();
    display.drawString(4, 25, "Connecting to WiFi..");
    display.display();
  }
  Serial.println("Connected to the Wi-Fi network");
  display.clear();
  display.drawString(4, 25, "Connected to the Wi-Fi network");
  display.display();
}


// MQTT Broker
const char *mqtt_broker_ip = "192.168.178.71";
const char *topic = "esp32";
const int mqtt_port = 1883;
PubSubClient client(espClient);

void mqtt_setup() {
  client.setServer(mqtt_broker_ip, mqtt_port);
  client.setCallback(callback);
  while(!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The Client %s connects to a MQTT Broker\n", client_id.c_str());
    display.clear();
    display.drawString(4, 25, "The Client " +  String(client_id) +  "connects to a MQTT Broker\n");
    display.display();
    if(client.connect(client_id.c_str())) {
      Serial.println("MQTT Broker connected");
      display.clear();
  display.drawString(4, 25, "MQTT Broker connected");
  display.display();
    }else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      display.clear();
  display.drawString(4, 25, "failed with state " + String(client.state()));
  display.display();
      delay(2000);
    }
  }
  client.publish(topic, "Hi, I'm ESP32 ^^");
  client.subscribe(topic);
}

// Ultrasonic
const int TRIG_PIN = 12;                                    // HC-SR04 Trig pin
const int ECHO_PIN = 13;                                    // HC-SR04 Echo pin
Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN);

void display_setup() {
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  delay(50);
  digitalWrite(16, HIGH);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
}



void setup() {
  
  Serial.begin(9600);
  display_setup();
  wifi_setup();
  mqtt_setup();
}

void loop() {
  client.loop();
  display.clear();

  int distance = ultrasonic.read();
  display.drawString(4, 25, String(distance) + " cm");

  if(client.connected()){
    String jsonData = "{\"distance\": " + String(distance) + "}";
    client.publish(topic, jsonData.c_str());
    display.drawString(4, 35, "Sending data");
  }else {
    display.drawString(4, 35, "Stopped sending data");
  }

  display.display();
  delay(1000);
}
