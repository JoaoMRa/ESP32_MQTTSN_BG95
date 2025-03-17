# ESP32_MQTTSN_BG95

Biblioteca para comunicação MQTT-SN com módulos **Quectel BG95** em **ESP32**, compatível com **Arduino** e **MicroPython**.

## 🚀 Funcionalidades
- Comunicação serial com o BG95 usando AT Commands.
- Suporte para conexão a um **broker MQTT-SN**.
- Publicação e inscrição em tópicos.
- Suporte para GPS e UDP.

## 📦 Instalação

### Para **Arduino**
1. Baixe ou clone este repositório.
2. Copie a pasta `ESP32_MQTTSN_BG95` para a pasta `libraries` do Arduino (`~/Documents/Arduino/libraries/`).
3. Reinicie o Arduino IDE.

### Para **MicroPython**
1. Suba o arquivo `ESP32_MQTTSN_BG95.py` para o ESP32 via **Thonny** ou **mpy-cross**.
2. Importe a biblioteca no seu código:

```python
from MQTTSN_BG95 import MQTTSN_BG95
