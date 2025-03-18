import machine
import time

class ESP32_MQTTSN_BG95:
    def __init__(self, uart_id=1, baudrate=115200, tx=17, rx=16):
        self.uart = machine.UART(uart_id, baudrate=baudrate, tx=tx, rx=rx)
        self.uart.init(baudrate, bits=8, parity=None, stop=1)

    def send_at_command(self, command, timeout=2000):
        self.uart.write(command + "\r\n")
        time.sleep_ms(100)
        
        response = b""
        start = time.ticks_ms()
        
        while time.ticks_diff(time.ticks_ms(), start) < timeout:
            if self.uart.any():
                response += self.uart.read()
        
        response_str = response.decode('utf-8') if response else ""
        print(f"Comando: {command}")
        print(f"Resposta: {response_str}")
        return response_str

    def connect_apn(self, id, apn):
        self.send_at_command(f'AT+CGDCONT={id},"IP","{apn}"')

    def apn_verify(self):
        self.send_at_command("AT+CGDCONT?")

    def connect_broker(self, client_id, broker, port):
        self.send_at_command(f'AT+QMTSNOPEN={client_id},"{broker}",{port},"clientid"')

    def broker_verify(self):
        self.send_at_command("AT+QMTSNOPEN?")

    def close_broker(self):
        self.send_at_command("AT+QMTSNCLOSE=0")

    def connect_mqttsn(self, client_id, user, password):
        self.send_at_command(f'AT+QMTCONN="{client_id}","{user}","{password}"')

    def mqttsn_verify(self):
        self.send_at_command("AT+QMTCONN?")

    def close_mqttsn(self, client_id):
        self.send_at_command(f"AT+QMTDISC={client_id}")

    def activate_udp(self):
        self.send_at_command("AT+CGACT=1,1")
        self.send_at_command('AT+QMTSNCFG="dtls",0,0')

    def activate_gps(self):
        self.send_at_command("AT+QGPS=1")

    def deactivate_gps(self):
        self.send_at_command("AT+QGPSEND")

    def gps_coordinates(self):
        self.send_at_command("AT+QGPSLOC?")

    def publish(self, message, client_id, topic, msg_id, qos, retain, msglen):
        self.send_at_command(f'AT+QMTPUB={client_id},{msg_id},{qos},{retain},"{topic}",{msglen}')
        self.uart.write(message + "\r\n")

    def subscribe(self, client_id, msg_id, topic, qos):
        self.send_at_command(f'AT+QMTSNSUB={client_id},{msg_id},"{topic}",{qos}')

    def unsubscribe(self, client_id, msg_id, topic):
        self.send_at_command(f'AT+QMTSNUNS={client_id},{msg_id},"{topic}"')

    def request_topic_id(self):
        self.send_at_command("AT+QMTSNREG?")

    def will_update(self, client_id, will_qos, will_retain, will_topic, will_message):
        self.send_at_command(f'AT+QMTSNWILLUPD={client_id},{will_qos},{will_retain},"{will_topic}","{will_message}"')

    def module_mode(self, client_id, mode, duration=None):
        if duration:
            self.send_at_command(f"AT+QMTSNSLEEP={client_id},{mode},{duration}")
        else:
            self.send_at_command(f"AT+QMTSNSLEEP={client_id},{mode}")
