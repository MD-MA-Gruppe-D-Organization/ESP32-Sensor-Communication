#include <Wifi.h>
#include <PubSubClient.h>
#include "wifi_credentials.h"
#include <Ultrasonic.h>
#include <Arduino.h>
#include <Wire.h>
#include <SSD1306Wire.h>
#include <ArduinoJson.h>
#include <cstdlib>
#include <iostream>
String default_topic;
 int rand_id;
JsonDocument doc; // Adjust size according to your needs

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
}

// Display
SSD1306Wire display(0x3c, 4, 15);

// WiFi
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;
WiFiClient espClient;

void wifi_setup()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
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

void mqtt_setup()
{
  client.setServer(mqtt_broker_ip, mqtt_port);
  client.setCallback(callback);
  while (!client.connected())
  {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The Client %s connects to a MQTT Broker\n", client_id.c_str());
    display.clear();
    display.drawString(4, 25, "The Client " + String(client_id) + "connects to a MQTT Broker\n");
    display.display();
    if (client.connect(client_id.c_str()))
    {
      Serial.println("MQTT Broker connected");
      display.clear();
      display.drawString(4, 25, "MQTT Broker connected");
      display.display();
    }
    else
    {
      Serial.print("failed with state ");
      Serial.print(client.state());
      display.clear();
      display.drawString(4, 25, "failed with state " + String(client.state()));
      display.display();
      delay(2000);
    }
  }
}

// Ultrasonic
const int TRIG_PIN = 12; // HC-SR04 Trig pin
const int ECHO_PIN = 13; // HC-SR04 Echo pin
Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN);

void display_setup()
{
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  delay(50);
  digitalWrite(16, HIGH);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
}

void setup()
{
  Serial.begin(9600);
  delay(2000); // Add a delay to ensure the serial terminal is ready

  display_setup();
  wifi_setup();
  mqtt_setup();

  rand_id = std::rand();
  doc["id"] = rand_id;
  default_topic = "mdma/" + String(rand_id);

  Serial.println("Setup complete");
}

void loop()
{
  client.loop();

  int distance = ultrasonic.read(CM);
  if (distance == 0) {
    Serial.println("Ultrasonic sensor error");
  }

  doc["data"] = distance;
  String jsonString;
  serializeJson(doc, jsonString);

  if (client.connected()) {
    if (client.publish(default_topic.c_str(), jsonString.c_str())) {
      Serial.println("Published data successfully");
      Serial.println(jsonString.c_str());
    } else {
      Serial.println("Failed to publish data");
    }
  } else {
    Serial.println("MQTT client not connected");
  }

  display.clear();
  display.drawString(4, 15, String(distance) + " cm");
  display.drawString(4, 25, client.connected() ? "Sending data..." : "Stopped sending data");
  display.drawString(4, 35, String(rand_id));
  display.display();

  delay(1000);
}
