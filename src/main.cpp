#include <Arduino.h>
#include <Ultrasonic.h>
#include <WiFi.h>


/////// DISPLAY STUFF
#include <Wire.h>               
#include "SSD1306Wire.h"        
SSD1306Wire display(0x3c, 4, 15);                           // ADDRESS, SDA, SCL
///////

/////// WIFI STUFF
const char* ssid = "Vodafone-560C";
const char* password = "zhxFyfHKaR47eN4q";

/* const char* ssid = "Galaxy S22 661F";                    // Mobile Hotspot Julian
const char* password = "uolp1538"; */
///////

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
  Serial.println("\nscan start");
  int n = WiFi.scanNetworks();
  Serial.println("\nscan done\n");

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
  Serial.println("");
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
      Serial.print("Local ESP32 IP: ");
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

void setup() {

  initDisplay();
  pinMode(TRIG_PIN, OUTPUT);                                              // Set TRIG_PIN (pin 12) as output
  pinMode(ECHO_PIN, INPUT);                                               // Set ECHO_PIN (pin 13) as input
  Serial.begin(9600);

  connectToWifi();
  Serial.print("\n\n"); 
  Serial.print("Received Signal Strength Indication (RRSI): ");                             
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
    Serial.print(WiFi.RSSI());                                          // prints the wifi connection strength
  }
}