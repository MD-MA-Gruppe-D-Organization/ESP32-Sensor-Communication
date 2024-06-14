#include <Wifi.h>
#include <PubSubClient.h>
#include "wifi_credentials.h"
#include <Ultrasonic.h>
#include <Arduino.h>
#include <Wire.h>
#include "SSD1306Wire.h"
#include <ArduinoJson.h>
#include "UUID.h"

#define DEFAULT_TOPIC "mdma/id_of_sensor/building/location/measurement"
#define SETTING_TOPIC "mdma/id_of_sensor/settings"

String default_topic;
String settings_topic;

struct Sensor
{
  String id;
};
Sensor sensor;

struct Settings
{
  String building;
  String location;
  String unit;
  String sensorType;
  String totalMax;
  String minValue;
  String maxValue;
  bool active;
  String measurement;
};
Settings settings;

JsonDocument doc;          // Adjust size according to your needs
JsonDocument doc_settings; // Adjust size according to your needs
void callback(char *topic, byte *payload, unsigned int length)
{

  DeserializationError error = deserializeJson(doc, payload);

  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  if (strcmp(topic, settings_topic.c_str()) == 0)
  {
    settings.building = doc["settings"]["building"].as<String>();
    settings.location = doc["settings"]["location"].as<String>();
    settings.unit = doc["settings"]["unit"].as<String>();
    settings.sensorType = doc["settings"]["sensor_type"].as<String>();
    settings.totalMax = doc["settings"]["total_max"].as<String>();
    settings.minValue = doc["settings"]["min_value"].as<String>();
    settings.maxValue = doc["settings"]["max_value"].as<String>();
    settings.active = doc["settings"]["active"].as<bool>();
    settings.measurement = doc["settings"]["measurement"].as<String>();
  }
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
  display_setup();
  wifi_setup();
  mqtt_setup();

  sensor.id = random();
  settings.building = "unknown";
  settings.location = "unknown";
  settings.unit = "cm";
  settings.sensorType = "unknown";
  settings.totalMax = "357";
  settings.minValue = "0";
  settings.maxValue = settings.totalMax;
  settings.active = true;
  settings.measurement = "fill_level";

  default_topic = "mdma/" + sensor.id;
  settings_topic = "mdma/" + sensor.id + "/settings";

  client.subscribe(settings_topic.c_str());
}

void loop()
{
  client.loop();
  display.clear();
  int distance = 0;
  if(settings.active){
    int unit = strcmp(settings.unit.c_str(), String("cm").c_str()) == 0 ? CM : INC;
  distance = ultrasonic.read(unit);
  }
  

  doc["id"] = sensor.id;
  doc["data"] = distance;
  doc["settings"]["building"] = settings.building;
  doc["settings"]["location"] = settings.location;
  doc["settings"]["unit"] = settings.unit;
  doc["settings"]["sensor_type"] = settings.sensorType;
  doc["settings"]["total_max"] = settings.totalMax;
  doc["settings"]["min_value"] = settings.maxValue;
  doc["settings"]["max_value"] = settings.maxValue;
  doc["settings"]["active"] = settings.active;
  doc["settings"]["measurement"] = settings.measurement;
  String jsonString;
  serializeJson(doc, jsonString);
  
  if (client.connected() && settings.active)
  {
    client.publish(default_topic.c_str(), jsonString.c_str());
    display.drawString(4, 15, String(distance) + String(strcmp(settings.unit.c_str(), String("cm").c_str()) == 0 ? " cm" : " inches"));
    display.drawString(4, 25, "Sending data");
    display.drawString(4, 35, sensor.id);
  }
  else if(!client.connected())
  {
    display.drawString(4, 25, "Stopped sending data");
    display.drawString(4, 35, sensor.id);
  }else {
    display.drawString(4, 25, "Deactivated");
    display.drawString(4, 35, sensor.id);
  }

  display.display();
  delay(1000);
}
