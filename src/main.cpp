#include <Arduino.h>

#include <Ultrasonic.h>


const int TRIG_PIN = 12;    // HC-SR04 Trig pin
const int ECHO_PIN = 13;    // HC-SR04 Echo pin

Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN);	



void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  digitalWrite(LED_BUILTIN, HIGH);

  Serial.print("Sensor: ");
  Serial.print(ultrasonic.read()); // Prints the distance on the default unit (centimeters)
  Serial.println("cm");
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);


}
