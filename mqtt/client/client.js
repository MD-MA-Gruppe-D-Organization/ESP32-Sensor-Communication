const repl = require("node:repl");
const mqtt = require("mqtt");
const client = mqtt.connect("mqtt://mosquitto");

// print connected message
client.on("connect", () => {
    console.log("connected to mqtt://mosquitto", client);
    
});

// print error message
client.on("error", (err) => {
    console.error(err);
});

// called when message is received
client.on("message", function (topic, message) {
    console.log(`received message for topic "${topic}"`);
    console.log(message.toString());
});

// shortcut to subscribe to a topic
const subscribe = (topic, cb) => {
    client.subscribe(topic, cb);
};

// shortcut to publish to a topic
const publish = (topic, message, options, cb) => {
    client.publish(topic, message, options, cb);
};

// start accepting user input
const r = repl.start("> ");
r.context.client = client;
r.context.subscribe = subscribe;
r.context.publish = publish;
