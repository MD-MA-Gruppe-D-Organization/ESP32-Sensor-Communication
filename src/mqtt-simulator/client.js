import mqtt from 'mqtt';
import fs from 'fs';

// Load the configuration file
const config = JSON.parse(fs.readFileSync('config.json', 'utf8'));

// Extract configuration values
const mqttBrokerIP = config.mqtt_broker_ip;
const mqttPort = config.mqtt_port;
const mqttTopic = config.mqtt_topic;
const MAX_DATA_VALUE = 357;
const MAX_ID_VALUE = 10000;
const NUM_MEASUREMENTS = 5;

const url = `mqtt://${mqttBrokerIP}:${mqttPort}`;

const options = {
    clean: true,
    connectTimeout: 4000,
    reconnectPeriod: 5000 // Attempt to reconnect every 5 seconds
};

const client = mqtt.connect(url, options);

let topics = [];

client.on('connect', () => {
    console.log('Connected to the MQTT broker');

    // Generate random topics and subscribe to them
    for (let i = 0; i < NUM_MEASUREMENTS; i++) {
        const randomId = Math.floor(Math.random() * MAX_ID_VALUE);
        const topic = `${mqttTopic}/${randomId}`;
        topics.push(topic);

        client.subscribe(topic, (err) => {
            if (err) {
                console.error(`Failed to subscribe to topic: ${topic}`, err);
            } else {
                console.log(`Subscribed to topic: ${topic}`);
            }
        });
    }
});

client.on('error', (err) => {
    console.error('MQTT client error:', err);
});

client.on('reconnect', () => {
    console.log('Attempting to reconnect to the MQTT broker...');
});

client.on('offline', () => {
    console.error('MQTT client is offline');
});

client.on('close', () => {
    console.log('MQTT connection closed');
});

setInterval(() => {
    if (client.connected) {
        for (let i = 0; i < NUM_MEASUREMENTS; i++) {
            const randomData = Math.floor(Math.random() * MAX_DATA_VALUE);
            const messageJson = {
                id: topics[i].split('/').pop(), // Extract the randomId from the topic
                data: randomData
            };
            client.publish(topics[i], JSON.stringify(messageJson), (err) => {
                if (err) {
                    console.error('Failed to publish message', err);
                } else {
                    console.log('Published message:', messageJson);
                }
            });
        }
    } else {
        console.error('MQTT client is not connected.');
    }
}, 10000);

// Receive messages
client.on('message', (topic, message) => {
    console.log(`Received message from topic: ${topic}, message: ${message.toString()}`);
    // Optionally handle the received message
});
