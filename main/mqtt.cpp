#include "Arduino.h"
#include "mqtt.h"


MqttJson::MqttJson(PubSubClient& mqtt_client) : client(mqtt_client) {}


void MqttJson::init(const char* mqtt_server, const char* mqtt_user, const char* mqtt_password, const char* mqtt_id) {
  Serial.println("Mqtt init");
  _mqtt_server = mqtt_server;
  _mqtt_user = mqtt_user;
  _mqtt_password = mqtt_password;
  _mqtt_id = mqtt_id;
  client.setServer(_mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect();
}

void MqttJson::callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void MqttJson::reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(_mqtt_id, _mqtt_user, _mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("mq2_mqtt", "hello world init");
      // ... and resubscribe
      client.subscribe("subscribe_topic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

bool MqttJson::connected() {
  return client.connected();
}
