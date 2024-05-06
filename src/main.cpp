#include <Arduino.h>
#include <Ultrasonic.h>

const int TRIG_PIN = 12;    // HC-SR04 Trig pin
const int ECHO_PIN = 13;    // HC-SR04 Echo pin

Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN);	
#define MAX_DISTANCE 200
 


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);  // Set TRIG_PIN (pin 12) as output
  pinMode(ECHO_PIN, INPUT);   // Set ECHO_PIN (pin 13) as input
  Serial.begin(9600);
}

void loop() {

  digitalWrite(LED_BUILTIN, HIGH);

  Serial.print("Sensor: ");
  Serial.print(ultrasonic.read()); // Prints the distance on the default unit (centimeters)
  //Serial.print(ultrasonic.distanceRead());
  Serial.println("cm");
  
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
  delay(950);
}
