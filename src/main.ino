#include <WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>
#include "config.h"

#define power_pin 5

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  pinMode(power_pin, OUTPUT);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");

  char message = '1';
  for (int i = 0; i < length; i++)
  {
    Serial.println((char)payload[i]);
    message = (char)payload[i];
  }
  
  // control the machine
  if (String(topic) == control_topic){
    if(message == '0'){
      // Serial.println("machine is on");
      digitalWrite(power_pin, LOW);
    }
    else if (message == '1'){
      // Serial.println("machine is off");
      digitalWrite(power_pin, HIGH);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect, add the connection authenticate if require
    if (client.connect(wsmid, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      // Subscribe to control sth
      client.subscribe(control_topic);
    } else {
      Serial.print("failed, rc= ");
      Serial.println(client.state());
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}