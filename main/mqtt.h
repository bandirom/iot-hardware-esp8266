#ifndef MQTT_JSON_h
#define MQTT_JSON_h
#include <PubSubClient.h>

#include "Arduino.h"

class MqttJson {
  
  public:
    MqttJson(PubSubClient& mqtt_client);
    PubSubClient& client;
    void init(const char* mqtt_server, const char* mqtt_user, const char* mqtt_password, const char* mqtt_id);
    void reconnect();
//    void setSubscribeTopic();
    bool connected();
    bool loop();
    bool publish(const char* topic, const char* payload);
    bool subscribe(const char* topic);
    MqttJson& setDeviceUUID(const char* device_uuid);
  private:
    const int mqtt_port = 1883;
    static void callback(char* topic, byte* payload, unsigned int length);
    const char* _mqtt_server;
    const char* _mqtt_user;
    const char* _mqtt_password;
    const char* _mqtt_id;
    const char* uuid;
    const char* subscribe_topic;
};

#endif
