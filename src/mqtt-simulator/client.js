import mqtt from 'mqtt'

const url = 'mqtt://192.168.178.71:1883'
const MAX_DATA_VALUE = 357
const MAX_ID_VALUE = 10000

const options = {
    clean: true,
    connectTimeout: 4000
}


const randomId = Math.floor(Math.random() * MAX_ID_VALUE)
const topic = `mdma/${randomId}`

const client = mqtt.connect(url, options)
client.on('connect', () => {
    console.log('connected')

    client.subscribe(topic)
})

setInterval(() => {
    if (client.connected) {
        const randomData = Math.floor(Math.random() * MAX_DATA_VALUE);
        const messageJson = {
            id: randomId,
            data: randomData
        };
        client.publish(topic, JSON.stringify(messageJson));
    } else {
        console.error("MQTT client is not connected.");
    }
}, 10000);


// Receive messages
client.on('message', function (topic, message) {

    console.log(`topic: ${topic}, msg: ${message}`)
    //client.end()
})