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
    bg95Serial.print(command);
    bg95Serial.print("\r");

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

String ESP32_MQTTSN_BG95::sendATCommand(const String &command, const String &payload, unsigned long timeout) {
    String response = "";

    bg95Serial.print(command);
    bg95Serial.print("\r");

    delay(100); // Pequena espera para o módulo aceitar o payload

    // Enviar o payload
    bg95Serial.print(payload);
    bg95Serial.write(0x1A); // Envia Ctrl+Z

    unsigned long start = millis();
    while (millis() - start < timeout) {
        while (bg95Serial.available()) {
            response += (char)bg95Serial.read();
        }
    }

    Serial.println("Comando: " + command);
    Serial.println("Payload: " + payload);
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
    sendATCommand("AT+QMTSNCFG=\"timeout\",1,60,3,1");

}


// Conectar ao Broker MQTT-SN
void ESP32_MQTTSN_BG95::ConnectBroker(String clientId, String broker, String port, String client_name) {
    this->clientId = clientId;
    this->broker = broker;
    this->port = port;
    sendATCommand("AT+QMTSNOPEN=" + clientId + ",\"" + broker + "\"," + port + ",\"" + client_name + "\"");
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
void ESP32_MQTTSN_BG95::publish(String message, String clientId, String topic, String msgId, String qos, String retain) {
    int msglen = message.length();  // Calcula o comprimento da mensagem

    String Command("AT+QMTSNPUB=" + clientId + "," + msgId + "," + qos + "," + retain + ",\"" + topic + "\"," + String(msglen));
    sendATCommand(Command, message);
    
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
void ESP32_MQTTSN_BG95::RegTopic(String clientId, String msgId, String topic){
    sendATCommand("AT+QMTSNREG="+ clientId + ","+ msgId +",\""+ topic+ "\"");
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

void ESP32_MQTTSN_BG95::parse_command_line(String& response) {
    response.trim();

    while (response.startsWith("\n") || response.startsWith("\r")) {
        response = response.substring(1);
        response.trim();
    }
    while (response.endsWith("\n") || response.endsWith("\r")) {
        response = response.substring(0, response.length() - 1);
        response.trim();
    }


}

bool ESP32_MQTTSN_BG95::check_command(String command, String result, uint32_t wait){
    String response = sendATCommand(command,2000);
    uint32_t timeout = millis() + wait;
    bool expect_response = false;

    while(millis() <= timeout){
        #ifdef DEBUG_BG95_HIGH
            log("<< " + response);
        #endif

        parse_command_line(response);

        if (response == result)
        {
            expect_response = true;
            break;
        }

        if (response == "ERROR")
        {
            break;
        }

        delay(2000);

    }
    return expect_response;
}

bool ESP32_MQTTSN_BG95::waitForResponse(String expected, unsigned long timeout) {
  unsigned long startTime = millis();
  String response = "";

  while (millis() - startTime < timeout) {
    while (bg95Serial.available()) {
      char c = bg95Serial.read();
      response += c;

      // Opcional: mostrar resposta parcial
      Serial.print(c);

      // Verifica se a resposta esperada foi recebida
      if (response.indexOf(expected) != -1) {
        Serial.println("\nResposta esperada recebida: " + expected);
        return true;
      }
    }
  }

  Serial.println("\nTimeout à espera de: " + expected);
  return false;
}

bool ESP32_MQTTSN_BG95::getValidGPSCoordinates(String &latitude, String &longitude, int maxAttempts) {
    for (int i = 0; i < maxAttempts; i++) {
        Serial.println("Tentativa de obter coordenadas GPS válida...");

        String response = sendATCommand("AT+QGPSLOC?", 5000);

        int index = response.indexOf("+QGPSLOC:");
        if (index != -1 && response.indexOf("516") == -1) {
            // Encontrou uma resposta válida
            String data = response.substring(index + 10);
            data.trim();

            int firstComma = data.indexOf(',');
            if (firstComma != -1) {
                latitude = data.substring(0, firstComma);
                int secondComma = data.indexOf(',', firstComma + 1);
                if (secondComma != -1) {
                    longitude = data.substring(firstComma + 1, secondComma);
                    Serial.println("Coordenadas GPS obtidas com sucesso:");
                    Serial.println("Latitude: " + latitude);
                    Serial.println("Longitude: " + longitude);
                    return true;
                }
            }
        }

        Serial.println("Resposta inválida. Repetindo tentativa...");
        delay(2000); // Aguarda antes de tentar novamente
    }

    Serial.println("Falha ao obter coordenadas GPS após " + String(maxAttempts) + " tentativas.");
    return false;
}

