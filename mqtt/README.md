
## Run Locally

Start MQTT Server

```bash
docker compose up
[+] Running 3/3
 ⠿ Network iot_default        Created                                                    0.0s
 ⠿ Container iot-client-1     Created                                                    0.0s
 ⠿ Container iot-mosquitto-1  Created                                                    0.1s
Attaching to iot-client-1, iot-mosquitto-1
iot-mosquitto-1  | 1682326817: mosquitto version 2.0.15 starting
iot-mosquitto-1  | 1682326817: Config loaded from /mosquitto/config/mosquitto.conf.
iot-mosquitto-1  | 1682326817: Opening ipv4 listen socket on port 1883.
iot-mosquitto-1  | 1682326817: Opening ipv6 listen socket on port 1883.
iot-mosquitto-1  | 1682326817: Opening websockets listen socket on port 9001.
iot-mosquitto-1  | 1682326817: mosquitto version 2.0.15 running
iot-mosquitto-1  | 1682326817: New connection from 172.19.0.2:44874 on port 1883.
iot-mosquitto-1  | 1682326817: New client connected from 172.19.0.2:44874 as mqttjs_369611c9 (p2, c1, k60).
iot-client-1     | > connected to mqtt://mosquitto                                                              0.3s
```

For testing purposes, go to the client folder

```bash
docker compose run client
> connected to mqtt://mosquitto
> 
```

### Example

subscribe (client)
```bash
docker compose run client
> connected to mqtt://mosquitto
> subscribe("rooms/garage/#")
```
publish (client)
```bash
publish("rooms/garage/temperature", message);
```

