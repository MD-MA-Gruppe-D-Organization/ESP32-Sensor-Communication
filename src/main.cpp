#include <Arduino.h>
#include <Ultrasonic.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

/////// DISPLAY STUFF
#include <Wire.h>               
#include "SSD1306Wire.h"        
SSD1306Wire display(0x3c, 4, 15);                           // ADDRESS, SDA, SCL
///////

/////// WIFI & MQTT STUFF
const char* ssid = "Galaxy S22 661F";                       // Mobile Hotspot Julian
const char* password = "uolp1538";

const char* mqtt_server = "a367aa99f5bd41b8aaaf16c83e749b9f.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;  
const char* topic = "Distance";
const char* mqtt_username = "jertle";
const char* mqtt_password = "6EAzFg2EU5&hcSkrmfSw";

WiFiClientSecure espClient;
PubSubClient client(espClient);

// HiveMQ Cloud Let's Encrypt CA certificate
static const char *root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";
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

void reconnectCallback(char* topic, byte* payload, unsigned int length) {
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
    if (client.connect(clientId.c_str(),mqtt_username,mqtt_password)) {
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
  espClient.setCACert(root_ca);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(reconnectCallback);                                  // Optional callback for reconnection logic
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
}