#ifndef ESP32_MQTTSN_BG95_H
#define ESP32_MQTTSN_BG95_H

#include "Arduino.h"
#include <HardwareSerial.h>

class ESP32_MQTTSN_BG95 {
  private:
    HardwareSerial& bg95Serial;
    String clientId;
    String broker;
    String port;
    String user;
    String pass;
    String will_qos;
    String will_retain;
    String will_topic;
    String will_message;

    

  public:
    ESP32_MQTTSN_BG95(HardwareSerial& serial);

    void configSerial(int baudrate, HardwareSerial& name, int Rx, int Tx);
    mqttsn.sendATCommand("AT+QMTSNOPEN=1,\"ip\",1884,\"client\"");

    String sendATCommand(const String &command, const String &payload, unsigned long timeout = 2000);

    void ConnectApn(String Id, String apn);
    void ApnVerify();
    bool waitForGPSFix(unsigned long timeout_ms);
    void ConfNTN(String Id, String apn);

    void ConfIOT(String Id, String apn);

    void ConnectBroker(String clientId, String broker, String port, String client_name);
    void BrokerVerify();
    void CloseBroker();
    
    void connectMQTTSN(String clientId, String user, String pass);
    void MQTTSNVerify();
    void CloseMQTTSN(String clientId);
    
    void AtivateUdp();
    void ActivateGps();
    void DesactivateGps();
    void GpsCordinates();
    
    void publish(String message, String clientId, String topic, String msgId, String qos, String retain);
    void Subscribe(String clientId, String msgId, String topic, String qos);
    void Unsubscribe(String clientId, String msgId, String topic);
    void RequestTopicId();
    void RegTopic(String clientId, String msgId, String topic);
    
    void WillUpd();
    void ModuleMode1or2(String clientId, String mode);
    void ModuleMode0(String clientId, String mode, String duration);

    void parse_command_line(String& response);
    bool check_command(String command, String result, uint32_t wait);
    bool waitForResponse(String expected, unsigned long timeout = 5000);
      
};

#endif

