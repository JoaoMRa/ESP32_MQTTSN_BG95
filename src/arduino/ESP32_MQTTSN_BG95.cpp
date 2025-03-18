#include "ESP32_MQTTSN_BG95.h"

ESP32_MQTTSN_BG95::ESP32_MQTTSN_BG95(HardwareSerial& serial) : bg95Serial(serial) {}
//Configure Serial
void ESP32_MQTTSN_BG95::configSerial(int baudrate, HardwareSerial& name, int Rx, int Tx) {
    Serial.begin(baudrate);  // Monitor serial
    name.begin(baudrate, SERIAL_8N1, Rx, Tx);
}
//Send AT Commands
String ESP32_MQTTSN_BG95::sendATCommand(const String &command, unsigned long timeout) {
    String response = "";
    bg95Serial.println(command);
    unsigned long start = millis();

    while (millis() - start < timeout) {
        while (bg95Serial.available()) {
            response += (char)bg95Serial.read();
        }
    }

    Serial.println("Comando: " + command);
    Serial.println("Resposta: " + response);
    return response;
}

// Connect Apn
void ESP32_MQTTSN_BG95::ConnectApn(String Id, String apn) {
    sendATCommand("AT+CGDCONT=" + Id + ",\"IP\",\"" + apn + "\"");
    delay(1000);
}

// Show Apn state
void ESP32_MQTTSN_BG95::ApnVerify() {
    sendATCommand("AT+CGDCONT?");
}

// Connect MQTT-SN Broker 
void ESP32_MQTTSN_BG95::ConnectBroker(String clientId, String broker, String port) {
    this->clientId = clientId;
    this->broker = broker;
    this->port = port;
    sendATCommand("AT+QMTSNOPEN=" + clientId + ",\"" + broker + "\"," + port + ",\"clientid\"");
    delay(1000);
}

//Show MQTT-SN Broker state
void ESP32_MQTTSN_BG95::BrokerVerify() {
    sendATCommand("AT+QMTSNOPEN?");
}

// Close MQTT-SN Broker 
void ESP32_MQTTSN_BG95::CloseBroker() {
    sendATCommand("AT+QMTSNCLOSE=0");
}

// Connect to MQTT-SN
void ESP32_MQTTSN_BG95::connectMQTTSN(String clientId, String user, String pass) {
    this->user = user;
    this->pass = pass;
    sendATCommand("AT+QMTCONN=\"" + clientId + "\",\"" + user + "\",\"" + pass + "\"");
}

// show MQTT-SN state
void ESP32_MQTTSN_BG95::MQTTSNVerify() {
    sendATCommand("AT+QMTCONN?");
}

// Close MQTT-SN connection
void ESP32_MQTTSN_BG95::CloseMQTTSN(String clientId) {
    sendATCommand("AT+QMTDISC=" + clientId);
}

// UDP enable
void ESP32_MQTTSN_BG95::AtivateUdp() {
    sendATCommand("AT+CGACT=1,1");
    sendATCommand("AT+QMTSNCFG=\"dtls\",0,0");
}

// GPS enable
void ESP32_MQTTSN_BG95::ActivateGps() {
    sendATCommand("AT+QGPS=1");
}

// GPS disable
void ESP32_MQTTSN_BG95::DesactivateGps() {
    sendATCommand("AT+QGPSEND");
}

// GPS coordinates
void ESP32_MQTTSN_BG95::GpsCordinates() {
    sendATCommand("AT+QGPSLOC?");
}

// MQTT-SN publish
void ESP32_MQTTSN_BG95::publish(String message, String clientId, String topic, String msgId, String qos, String retain, String msglen) {
    sendATCommand("AT+QMTSNPUB=" + clientId + "," + msgId + "," + qos + "," + retain + ",\"" + topic + "\"," + msglen);
    bg95Serial.println(message);
    bg95Serial.println("");  // Enviar mensagem
}

// Subscrive MQTT-SN topic
void ESP32_MQTTSN_BG95::Subscribe(String clientId, String msgId, String topic, String qos) {
    sendATCommand("AT+QMTSNSUB=" + clientId + "," + msgId + ",\"" + topic + "\"," + qos);
}

// Unsubscive MQTT-SN topic
void ESP32_MQTTSN_BG95::Unsubscribe(String clientId, String msgId, String topic) {
    sendATCommand("AT+QMTSNUNS=" + clientId + "," + msgId + ",\"" + topic + "\"");
}

// Request topic ID 
void ESP32_MQTTSN_BG95::RequestTopicId() {
    sendATCommand("AT+QMTSNREG?");
}

// Update "Will" message
void ESP32_MQTTSN_BG95::WillUpd() {
    sendATCommand("AT+QMTSNWILLUPD=" + clientId + "," + will_qos + "," + will_retain + ",\"" + will_topic + "\",\"" + will_message + "\"");
}

// change Module mode (1 or 2)
void ESP32_MQTTSN_BG95::ModuleMode1or2(String clientId, String mode) {
    sendATCommand("AT+QMTSNSLEEP=" + clientId + "," + mode);
}

// change Module mode to 0 with duration
void ESP32_MQTTSN_BG95::ModuleMode0(String clientId, String mode, String duration) {
    sendATCommand("AT+QMTSNSLEEP=" + clientId + "," + mode + "," + duration);
}
