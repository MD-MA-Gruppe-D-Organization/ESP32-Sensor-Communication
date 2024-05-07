#include <Arduino.h>
#include <Ultrasonic.h>

/////// DISPLAY STUFF
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
SSD1306Wire display(0x3c, 4, 15);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h
///////

const int TRIG_PIN = 12;    // HC-SR04 Trig pin
const int ECHO_PIN = 13;    // HC-SR04 Echo pin
String distanceString = "Waiting for measurement";

Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN);
#define MAX_DISTANCE 200
 

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


void setup() {

  initDisplay();
  pinMode(TRIG_PIN, OUTPUT);  // Set TRIG_PIN (pin 12) as output
  pinMode(ECHO_PIN, INPUT);   // Set ECHO_PIN (pin 13) as input
  Serial.begin(9600);
}


void loop() {
  display.clear();
  int distance = ultrasonic.read();

  if(distance<350){   // If illegal value, ignore it and print the value from before
    distanceString = String(distance) + "cm";
  }
  
  display.drawString(4, 25, distanceString);
  display.display();

  Serial.print("Sensor: ");
  Serial.print(distance); // Prints the distance on the default unit (centimeters)
  Serial.println("cm");
  
  delay(20);
}