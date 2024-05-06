#include <Wire.h>

#include "SSD1306Wire.h"
SSD1306Wire display(0x3C, 4, 15);



void setup_ssd1306()
{
    Serial.begin(921600); // Initialize serial communication at 115200 baud rate
    Wire.begin(21, 22);                // Initialize the I2C communication with the display
    display.init();                    // Initialize the display
    display.setFont(ArialMT_Plain_16); // Set the font for the display
    display.clear();                   // Clear the display
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 0, "Hello World");
    display.display();
}

void setup(){
  setup_ssd1306();
};
void loop(){
  Serial.println("Test");
  delay(1000);
  display.clear();                   // Clear the display
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, "Hello World");
  display.display();
};

