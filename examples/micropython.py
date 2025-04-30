from ESP32_MQTTSN_BG95 import ESP32_MQTTSN_BG95
import time

mqttsn = ESP32_MQTTSN_BG95(uart_id=1, baudrate=115200, tx=17, rx=16)

mqttsn.connect_apn("1", "internet.apn")
mqttsn.apn_verify()

mqttsn.conf_ntn(1,"apn_name")

mqttsn.connect_broker("client1", "mqtt.example.com", "1883")
mqttsn.broker_verify()

mqttsn.connect_mqttsn("client1", "user", "password")
mqttsn.mqttsn_verify()

mqttsn.publish("Hello, MQTT-SN!", "client1", "sensor/data", "1", "0", "0", "15")

mqttsn.subscribe("client1", "2", "sensor/data", "0")

while True:
    time.sleep(5)
    mqttsn.publish("Ping!", "client1", "sensor/data", "3", "0", "0", "5")
