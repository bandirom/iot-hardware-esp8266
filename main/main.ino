#include <ESP8266WiFi.h>
#include "credentials.h"
#include "mqtt.h"

WiFiClient espClient;
PubSubClient client(espClient);
MqttJson mqtt(client);

long lastMsg = 0;
char payload[700];

void setup_wifi() {
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void setup() {
  Serial.begin(115200);
  setup_wifi();
  mqtt.init(mqtt_server, mqtt_user, mqtt_password, mqtt_id);
  mqtt.setDeviceUUID(uuid);
  mqtt.subscribe(subscribe_topic);
}

void loop() {
  if (!mqtt.connected()) {
    mqtt.reconnect();
  }
  mqtt.loop();
  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    mqtt.publish(publish_topic, payload);
  }
}
