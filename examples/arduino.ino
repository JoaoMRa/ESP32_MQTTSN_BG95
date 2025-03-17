#include "ESP32_MQTTSN_BG95.h"

HardwareSerial bg95Serial(1);
ESP32_MQTTSN_BG95 mqttsn(bg95Serial);

void setup() {
    Serial.begin(115200);
    mqttsn.configSerial(115200, bg95Serial, 17, 16);

    mqttsn.ConnectApn("1", "internet.apn");
    mqttsn.ApnVerify();

    mqttsn.ConnectBroker("client1", "mqtt.example.com", "1883");
    mqttsn.BrokerVerify();

    mqttsn.connectMQTTSN("client1", "user", "password");
    mqttsn.MQTTSNVerify();

    mqttsn.publish("Hello, MQTT-SN!", "client1", "sensor/data", "1", "0", "0", "15");

    mqttsn.Subscribe("client1", "2", "sensor/data", "0");
}

void loop() {
    delay(5000);
    mqttsn.publish("Ping!", "client1", "sensor/data", "3", "0", "0", "5");
}
