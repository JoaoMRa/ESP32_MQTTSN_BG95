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

### Para **MicroPython**
1. Suba o arquivo `ESP32_MQTTSN_BG95.py` para o ESP32 via **Thonny** ou **mpy-cross**.
2. Importe a biblioteca no seu código:

```python
from MQTTSN_BG95 import MQTTSN_BG95
