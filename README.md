
# ESP32 Sensor Communication

This repository provides a setup for communicating with an ESP32 microcontroller equipped with WiFi and an HC-SR04 Ultrasonic Sensor. It uses Docker for running a local MQTT broker and includes scripts for flashing the ESP32.

This is part of our garbage level reading project where the Ultrasonic Sensor reads the remaining space of a garbage bin.




## Prerequisites

1.  Hardware:

- [ESP32 Dev Kit C V4](https://www.az-delivery.de/en/products/esp-32-dev-kit-c-v4)

- [HC-SR04 Ultrasonic Sensor](https://www.az-delivery.de/en/products/3er-set-hc-sr04-ultraschallmodule)

2.  Software:

- [Docker Desktop](https://www.docker.com/products/docker-desktop/)

- A code editor like [Visual Studio Code](https://code.visualstudio.com/)




## Setup Instructions
### 1. Docker Configuration
1. Open Docker Desktop and ensure it is running in the background.

### 2. Clone the Repository

[Documentation](https://linktodocumentation)

```bash
  git clone https://github.com/MD-MA-Gruppe-D-Organization/ESP32-Sensor-Communication.git
```
```bash
  cd ESP32-Sensor-Communication
```
### 3. Configure WiFi Credentials
Update the WiFi credentials in the following configuration files:

- src/config.h
- src/mqtt-simulator/config.json

Replace wifi_ssid and wifi_password with your network credentials. To get your mqtt_broker_ip, open a Command Prompt and run:

```bash
  ipconfig
```
Use the IPv4 address from the output.

### 4. Start Docker Containers
In the ESP32-Sensor-Communication directory, run:
```bash
  docker compose up -d
```

### 5. Access the Data Explorer
1. Open a web browser and go to [http://localhost:8086/signin](http://localhost:8086/signin)

2. Use the default credentials:
- Username: admin
- Password: groupD12345678

3. Navigate to the Data Explorer, select iot_data, and the relevant databases to see the received data from the MQTT broker.

### 6. Flashing the ESP32
1. Install the PlatformIO extension in Visual Studio Code.
2. Connect your ESP32 to your computer.
3. Open the ESP32-Sensor-Communication project in VS Code.
4. In the PlatformIO extension, select "Upload" to flash the code to the ESP32.

### 7. Manual MQTT Publish and Subscribe
To manually publish settings and check the configuration:

Publish Settings:
```bash
  mosquitto_pub -h <mqtt_broker_ip> -t mdma/1481765933/settings -m "{\"settings\":{\"building\":\"geibelstr.11\",\"location\":\"wg1\",\"unit\":\"cm\",\"sensor_type\":\"HC-SR04\",\"total_max\":\"357\",\"min_value\":\"0\",\"max_value\":\"357\",\"active\":true,\"measurement\":\"fill_level\"}}"
```

Subscribe to Check Configuration

```bash
  mosquitto_sub -h <mqtt_broker_ip> -t "mdma/#"
```


```bash
  mosquitto_sub -h <mqtt_broker_ip> -t "mdma/#"
```
Expected Output
```bash
  {
  "sensor_id": "1481765933",
  "data": "123",
  "settings": {
    "building": "geibelstr.11",
    "location": "wg1",
    "unit": "cm",
    "sensor_type": "HC-SR04",
    "total_max": "357",
    "min_value": "0",
    "max_value": "357",
    "active": true,
    "measurement": "fill_level"
  }
}

```

## Authors

- [@octokatherine](https://www.github.com/octokatherine)

