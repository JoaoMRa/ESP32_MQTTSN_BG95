# ESP32_MQTTSN_BG95

Library for **MQTT-SN communication** using the **Quectel BG95** module with **ESP32**, compatible with both **Arduino** and **MicroPython**.

## 🚀 Features
- Serial communication with the BG95 using **AT Commands**.
- Supports **MQTT-SN broker connection**.
- Publish and subscribe to topics.
- Includes **GPS and UDP support**.

## 📦 Installation

### For **Arduino**
1. Download or clone this repository.
2. Copy the `ESP32_MQTTSN_BG95` folder into your **Arduino libraries** folder (`~/Documents/Arduino/libraries/`).
3. Restart the **Arduino IDE**.

### For **MicroPython**
1. Upload the `ESP32_MQTTSN_BG95.py` file to your ESP32 using **Thonny** or **mpy-cross**.
2. Import the library in your code:

```python
from ESP32_MQTTSN_BG95 import ESP32_MQTTSN_BG95
```
## 📖 Usage Examples
### 🟢 Basic Example (Arduino)

```c++
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
}

void loop() {
    delay(5000);
}
```

### 🟢 Basic Example (MicroPython)

```python
from ESP32_MQTTSN_BG95 import ESP32_MQTTSN_BG95
import time

mqttsn = ESP32_MQTTSN_BG95(uart_id=1, baudrate=115200, tx=17, rx=16)
mqttsn.connect_apn("1", "internet.apn")
mqttsn.apn_verify()
mqttsn.connect_broker("client1", "mqtt.example.com", "1883")

while True:
    time.sleep(5)
```
### 🟡 Publishing Messages (Arduino)
```c++
#include "ESP32_MQTTSN_BG95.h"

HardwareSerial bg95Serial(1);
ESP32_MQTTSN_BG95 mqttsn(bg95Serial);

void setup() {
    Serial.begin(115200);
    mqttsn.configSerial(115200, bg95Serial, 17, 16);
    mqttsn.ConnectApn("1", "internet.apn");
    mqttsn.ConnectBroker("client1", "mqtt.example.com", "1883");
    mqttsn.publish("Hello, MQTT-SN!", "client1", "test/topic");
}

void loop() {}
```
### 🟡 Publishing Messages (MicroPython)
```python
from ESP32_MQTTSN_BG95 import ESP32_MQTTSN_BG95
import time

mqttsn = ESP32_MQTTSN_BG95(uart_id=1, baudrate=115200, tx=17, rx=16)
mqttsn.connect_apn("1", "internet.apn")
mqttsn.connect_broker("client1", "mqtt.example.com", "1883")
mqttsn.publish("Hello, MQTT-SN!", "client1", "test/topic")

while True:
    time.sleep(5)
```
### 🔵 Subscribing to a Topic (Arduino)
```c++
#include "ESP32_MQTTSN_BG95.h"

HardwareSerial bg95Serial(1);
ESP32_MQTTSN_BG95 mqttsn(bg95Serial);

void setup() {
    Serial.begin(115200);
    mqttsn.configSerial(115200, bg95Serial, 17, 16);
    mqttsn.ConnectApn("1", "internet.apn");
    mqttsn.ConnectBroker("client1", "mqtt.example.com", "1883");
    mqttsn.Subscribe("client1", "1", "test/topic", "0");
}

void loop() {}
```
### 🔵 Subscribing to a Topic (MicroPython)
```python
from ESP32_MQTTSN_BG95 import ESP32_MQTTSN_BG95
import time

mqttsn = ESP32_MQTTSN_BG95(uart_id=1, baudrate=115200, tx=17, rx=16)
mqttsn.connect_apn("1", "internet.apn")
mqttsn.connect_broker("client1", "mqtt.example.com", "1883")
mqttsn.subscribe("client1", "1", "test/topic", "0")

while True:
    time.sleep(5)
```


## 🛠 AT Commands Reference

| **Command**          | **Description**                                             |
|----------------------|-------------------------------------------------------------|
| `AT+CGDCONT?`        | Verify APN connection                                       |
| `AT+QMTSNOPEN?`      | Verify MQTT-SN broker connection                            |
| `AT+QMTCONN?`        | Check MQTT-SN session status                                |
| `AT+QMTPUB=`         | Publish a message to a topic                                |
| `AT+QMTSNSUB=`       | Subscribe to a topic                                        |
| `AT+QMTSNUNS=`       | Unsubscribe from a topic                                    |
| `AT+QGPS=1`          | Enable GPS                                                  |
| `AT+QGPSEND`         | Disable GPS                                                 |
| `AT+QGPSLOC?`        | Get GPS coordinates                                         |
| `AT+QMTSNSLEEP=`     | Mode: 0 - Asleep state  | 1 - Awake state 2 - Active state  |


## ⚖ License
This project is licensed under the MIT License - see the LICENSE.md file for details.

### 🔗 GitHub Repository: ESP32_MQTTSN_BG95

### ✉️ Maintainer: João Moreira
