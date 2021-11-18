#include "Arduino.h"
#include "mqtt.h"
#include <ArduinoJson.h>

StaticJsonDocument<256> doc;

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

bool MqttJson::loop() {
  return client.loop();
}

bool MqttJson::subscribe(const char* topic){
  subscribe_topic = topic;
  return client.subscribe(subscribe_topic);
}

MqttJson& MqttJson::setDeviceUUID(const char* device_uuid) {
  this->uuid = device_uuid;
  return *this;
}


bool MqttJson::publish(const char* topic, const char* payload) {
  float temperature = random(0, 50);
  int humidity = random(30, 99);
  char buffer[256];
  doc["uuid"] = uuid;
  doc["sensors"]["temperature"] = temperature;
  doc["sensors"]["humidity"] = humidity;
  Serial.println();
  Serial.println("Publish message");
  serializeJsonPretty(doc, Serial);
  size_t n = serializeJson(doc, buffer);
  return client.publish(topic, buffer, n);
}
