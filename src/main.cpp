#include <Arduino.h>
#include <Ultrasonic.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

/////// DISPLAY STUFF
#include <Wire.h>               
#include "SSD1306Wire.h"        
#include <ESP32Ping.h>
SSD1306Wire display(0x3c, 4, 15);                           // ADDRESS, SDA, SCL
///////

/////// WIFI & MQTT STUFF
const char* ssid = "Galaxy S22 661F";                       // Mobile Hotspot Julian
const char* password = "uolp1538";

//const char* mqtt_server = "192.168.0.37";                
const char* mqtt_server = "localhost";                
const int mqtt_port = 1883;  
const char* topic = "Distance";


WiFiClientSecure espClient;
PubSubClient client(espClient);

const int TRIG_PIN = 12;                                    // HC-SR04 Trig pin
const int ECHO_PIN = 13;                                    // HC-SR04 Echo pin
Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN);

String distanceString = "Placeholder";
#define MAX_DISTANCE 200
String get_wifi_status(int status){
    switch(status){
        case WL_IDLE_STATUS:
        return "WL_IDLE_STATUS";
        case WL_SCAN_COMPLETED:
        return "WL_SCAN_COMPLETED";
        case WL_NO_SSID_AVAIL:
        return "WL_NO_SSID_AVAIL";
        case WL_CONNECT_FAILED:
        return "WL_CONNECT_FAILED";
        case WL_CONNECTION_LOST:
        return "WL_CONNECTION_LOST";
        case WL_CONNECTED:
        return "WL_CONNECTED";
        case WL_DISCONNECTED:
        return "WL_DISCONNECTED";
    }
}

void initDisplay()
{
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  delay(50);
  digitalWrite(16, HIGH);
  display.init();
  display.flipScreenVertically();
  //display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
}

void scanForWifis(){
  delay(500);
  Serial.println("\nScan start\n");
  
  int n = WiFi.scanNetworks();
  

  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print("Networks found: ");
    Serial.println(n);
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
    }
  }
  Serial.println("\nScan done\n");
}

void connectToWifi(){
    
    WiFi.mode(WIFI_STA);
    delay(500);
    scanForWifis();                                                         // optional but thought it would be nice to see all wifis in range
    WiFi.begin(ssid, password);
    Serial.println("Connecting: \n");

    int i = 0;

    while(WiFi.status() != WL_CONNECTED && i < 10){
        Serial.print("| " + get_wifi_status(WiFi.status()));
        i++;
        delay(1000);
    }

    if(WiFi.status() == WL_CONNECTED){
      Serial.println("\n\nConnected to the WiFi network");
      Serial.print("Local ESP32 IP:");
      Serial.println(WiFi.localIP());                            
    }
    else{
      Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\nConnection Failed");
    }
}

void printDistanceToSerialMonitor(int distance){
  Serial.print("Sensor: ");
  Serial.print(distance);                                                  // Distance on default unit (centimeters)
  Serial.println("cm");
}

void reconnectCallbackk(char* topic, byte* payload, unsigned int length) {
  // Optional callback function to handle messages received from the broker
  // You can use this to implement logic based on received data
  Serial.print("Received message on topic: ");
  Serial.println(topic);
  Serial.print("Payload: ");
  Serial.write(payload, length);
  Serial.println();
}

void reconnect() {
  // Loop until reconnected
  while (!client.connected()) {
    Serial.println("\nAttempting MQTT connection... ");
    String clientId = "ESP32Client-";
    clientId += String(WiFi.macAddress());
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(3000);
    } 
  }
}

void setup() {

  initDisplay();
  pinMode(TRIG_PIN, OUTPUT);                                              // Set TRIG_PIN (pin 12) as output
  pinMode(ECHO_PIN, INPUT);                                               // Set ECHO_PIN (pin 13) as input
  Serial.begin(9600);

  connectToWifi(); 
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(reconnectCallbackk);                                  // Optional callback for reconnection logic
  }

void loop() {
  display.clear();
  int distance = ultrasonic.read();

  if(distance<350){                                                      // If illegal value, ignore it and print the value from before
    distanceString = String(distance) + "cm";
  }
  
  display.drawString(4, 25, distanceString);
  display.display();
  //printDistanceToSerialMonitor(distance);
  delay(500);                                                           // lowest is 20ms                 

  int wifiStatusInt = WiFi.status();                                    // call get_wifi_status(wifiStatusInt) if you want the corresponding string

  if(wifiStatusInt!=WL_CONNECTED){
    connectToWifi();
    Serial.println("");
  }
  else{
    if (!client.connected()) {                                          // check if mqtt broker is connected
      reconnect();
    }
    client.loop();

    Serial.print("                    Received Signal Strength Indication (RRSI): ");
    Serial.println(WiFi.RSSI());                                        // prints the wifi connection strength

    if (client.connected()) {                                           // publish to mqtt broker
      client.publish(topic, String(distance).c_str());                  
      Serial.println("Data sent: " + String(distance) + "cm");
    }
  } 
  
/*   const IPAddress remote_ip(192, 168, 0, 37);        

  Serial.print("Pinging ip ");
  Serial.println(remote_ip);
  
  if(Ping.ping(remote_ip)) {
    Serial.println("Success!!");
  } else {
    Serial.println("Error :(");
  }
 */

}