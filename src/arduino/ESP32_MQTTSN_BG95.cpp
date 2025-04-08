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

// Conectar à APN
void ESP32_MQTTSN_BG95::ConnectApn(String Id, String apn) {
    sendATCommand("AT+CGDCONT=" + Id + ",\"IP\",\"" + apn + "\"");
    delay(1000);
}

// Verificar conexão à APN
void ESP32_MQTTSN_BG95::ApnVerify() {
    sendATCommand("AT+CGDCONT?");
}

void ESP32_MQTTSN_BG95::ConfIOT(String Id, String apn){
    sendATCommand("AT+CGDCONT=" + Id + ",\"IP\",\"" + apn + "\"");
    sendATCommand("AT+CFUN=0");
    sendATCommand("AT+QCFG=\"nwscanmode\",3,1");
    sendATCommand("AT+QCFG=\"nwscanseq\",00,1");
    sendATCommand("AT+QCFG=\"band\",0,0,0x80000,1");
    sendATCommand("AT+QCFG=\"iotopmode\",1,1");
    sendATCommand("AT+CFUN=1");
    sendATCommand("ATE0");
    sendATCommand("AT+COPS=1,2,\"26803\",9");
    sendATCommand("AT+QNWINFO");
    delay(1000);
}

bool ESP32_MQTTSN_BG95::waitForGPSFix(unsigned long timeout_ms) {
    unsigned long start = millis();
    while (millis() - start < timeout_ms) {
        String response = sendATCommand("AT+QGPSLOC?");
        if (response.indexOf("+QGPSLOC:") != -1 && response.indexOf("516") == -1) {
            Serial.println("GPS fix recebido!");
            return true;
        }
        delay(2000);  // espera 2 segundos antes de tentar de novo
    }
    Serial.println("Timeout esperando fix do GPS");
    return false;
}

void ESP32_MQTTSN_BG95::ConfNTN(String Id, String apn){

    sendATCommand("at+cfun=0");
    sendATCommand("at+qcfg=\"band\",0,80000,80000,2,1");
    sendATCommand("at+qcfg=\"iotopmode\",3,1");
    sendATCommand("at+qcfg=\"nwscanmode\",3,1");
    sendATCommand("at+qcfg=\"dbgctl\",0  ");
    sendATCommand("at+cmee=2");
    sendATCommand("at+qgps=1");
    sendATCommand("at+qgpsloc?");
    waitForGPSFix(60000);
    sendATCommand("at+qgpsend");
    sendATCommand("AT+CGDCONT=" + Id + ",\"IP\",\"" + apn + "\"");
    sendATCommand("at+cfun=1");
    sendATCommand("at+cereg=4");
    sendATCommand("at+qeng=\"servingcell\"");
    sendATCommand("at+qnwinfo");
    sendATCommand("at+cops?");
    sendATCommand("at+qiact=1");
    sendATCommand("at+qiact?");
  
}


// Conectar ao Broker MQTT-SN
void ESP32_MQTTSN_BG95::ConnectBroker(String clientId, String broker, String port) {
    this->clientId = clientId;
    this->broker = broker;
    this->port = port;
    sendATCommand("AT+QMTSNOPEN=" + clientId + ",\"" + broker + "\"," + port + ",\"clientid\"");
    delay(1000);
}

// Verificar conexão com o Broker
void ESP32_MQTTSN_BG95::BrokerVerify() {
    sendATCommand("AT+QMTSNOPEN?");
}

// Fechar conexão com o Broker
void ESP32_MQTTSN_BG95::CloseBroker() {
    sendATCommand("AT+QMTSNCLOSE=0");
}

// Conectar ao MQTT-SN
void ESP32_MQTTSN_BG95::connectMQTTSN(String clientId, String user, String pass) {
    this->user = user;
    this->pass = pass;
    sendATCommand("AT+QMTCONN=\"" + clientId + "\",\"" + user + "\",\"" + pass + "\"");
}

// Verificar conexão MQTT-SN
void ESP32_MQTTSN_BG95::MQTTSNVerify() {
    sendATCommand("AT+QMTCONN?");
}

// Fechar conexão MQTT-SN
void ESP32_MQTTSN_BG95::CloseMQTTSN(String clientId) {
    sendATCommand("AT+QMTDISC=" + clientId);
}

// Ativar UDP
void ESP32_MQTTSN_BG95::AtivateUdp() {
    sendATCommand("AT+CGACT=1,1");
    sendATCommand("AT+QMTSNCFG=\"dtls\",0,0");
}

// Ativar GPS
void ESP32_MQTTSN_BG95::ActivateGps() {
    sendATCommand("AT+QGPS=1");
}

// Desativar GPS
void ESP32_MQTTSN_BG95::DesactivateGps() {
    sendATCommand("AT+QGPSEND");
}

// Obter coordenadas do GPS
void ESP32_MQTTSN_BG95::GpsCordinates() {
    sendATCommand("AT+QGPSLOC?");
}

// Publicar mensagem no MQTT-SN
void ESP32_MQTTSN_BG95::publish(String message, String clientId, String topic, String msgId, String qos, String retain, String msglen) {
    sendATCommand("AT+QMTPUB=" + clientId + "," + msgId + "," + qos + "," + retain + ",\"" + topic + "\"," + msglen);
    bg95Serial.println(message);
    bg95Serial.println("");  // Enviar mensagem
}

// Inscrever-se em um tópico MQTT-SN
void ESP32_MQTTSN_BG95::Subscribe(String clientId, String msgId, String topic, String qos) {
    sendATCommand("AT+QMTSNSUB=" + clientId + "," + msgId + ",\"" + topic + "\"," + qos);
}

// Cancelar inscrição em um tópico MQTT-SN
void ESP32_MQTTSN_BG95::Unsubscribe(String clientId, String msgId, String topic) {
    sendATCommand("AT+QMTSNUNS=" + clientId + "," + msgId + ",\"" + topic + "\"");
}

// Solicitar ID do Tópico MQTT-SN
void ESP32_MQTTSN_BG95::RequestTopicId() {
    sendATCommand("AT+QMTSNREG?");
}

// Atualizar mensagem de "Will"
void ESP32_MQTTSN_BG95::WillUpd() {
    sendATCommand("AT+QMTSNWILLUPD=" + clientId + "," + will_qos + "," + will_retain + ",\"" + will_topic + "\",\"" + will_message + "\"");
}

// Mudar o modo do módulo (1 ou 2)
void ESP32_MQTTSN_BG95::ModuleMode1or2(String clientId, String mode) {
    sendATCommand("AT+QMTSNSLEEP=" + clientId + "," + mode);
}

// Mudar o modo do módulo para 0 com duração
void ESP32_MQTTSN_BG95::ModuleMode0(String clientId, String mode, String duration) {
    sendATCommand("AT+QMTSNSLEEP=" + clientId + "," + mode + "," + duration);
}
