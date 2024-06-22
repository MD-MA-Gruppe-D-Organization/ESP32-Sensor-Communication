# ESP32-Sensor-Communication

Bauteilliste:

**ESP32 mit Wifi**
https://www.az-delivery.de/en/products/esp-32-dev-kit-c-v4

**Ultraschallsensor (HC-SR04)**
https://www.az-delivery.de/en/products/3er-set-hc-sr04-ultraschallmodule

**MQTT**

**_Messages_**

mdma/#

```json
{
  "sensor_id": "1481765933",
  "data": "123",
  "settings": {
    "building": "unknown",
    "location": "unknown",
    "unit": "cm", // cm or inc
    "sensor_type": "unknown",
    "total_max": "357",
    "min_value": "0",
    "max_value": "357",
    "active": true,
    "measurement": "fill_level"
  }
}
```

**_Settings_**

Configure the metadata

```bash
mosquitto_pub -h 192.168.178.71 -t mdma/1481765933/settings -m "{\"settings\":{\"building\":\"geibelstr.11\",\"location\":\"wg1\",\"unit\":\"cm\",\"sensor_type\":\"HC-SR04\",\"total_max\":\"357\",\"min_value\":\"0\",\"max_value\":\"357\",\"active\":true\"measurement\":\"fill_level\"}}"
```

Check if configuration was successful

```bash
mosquitto_sub -h 192.168.178.71 -t "mdma/#"
```

output

```json
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
