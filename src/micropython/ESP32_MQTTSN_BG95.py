import machine
import time

class ESP32_MQTTSN_BG95:
    def __init__(self, uart_id=2, baudrate=115200):
        self.uart = machine.UART(uart_id, baudrate=baudrate)
        self.uart.init(baudrate, bits=8, parity=None, stop=1)

    def send_at_command(self, command, timeout=5000):
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

    def wait_for_gps_fix(self,timeout_ms):
        t_start = time.ticks_ms()
        while time.ticks_diff(time.ticks_ms(), t_start) < timeout_ms:
            resp = self.send_at_command('AT+QGPSLOC?')
            if '+QGPSLOC:' in resp and '516' not in resp:
                print("GPS fix recebido!")
                return True
            time.sleep(2)
        print("Timeout esperando fix do GPS")
        return False

    def conf_iot(self,id, apn):
        self.send_at_command(f'AT+CGDCONT={id},"IP","{apn}"')
        self.send_at_command('AT+CFUN=0')
        self.send_at_command('AT+QCFG="nwscanmode",3,1')
        self.send_at_command('AT+QCFG="nwscanseq",00,1')
        self.send_at_command('AT+QCFG="band",0,0,0x80000,1')
        self.send_at_command('AT+QCFG="iotopmode",1,1')
        self.send_at_command('AT+CFUN=1')
        self.send_at_command('ATE0')
        self.send_at_command('AT+COPS=1,2,"26803",9')
        self.send_at_command('AT+QNWINFO')
        time.sleep(1)

    def conf_ntn(self,id, apn):
        self.send_at_command('AT+CFUN=0')
        self.send_at_command('at+qcfg="band",0,80000,80000,2,1')  # Confirme os valores de banda
        self.send_at_command('AT+QCFG="iotopmode",3,1')
        self.send_at_command('AT+QCFG="nwscanmode",3,1')
        self.send_at_command('AT+QCFG="dbgctl",0')
        self.send_at_command('AT+CMEE=2')
        self.send_at_command('AT+QGPS=1')

        self.wait_for_gps_fix(60000)  # espera até 60s por fix

        self.send_at_command('AT+QGPSEND')
        self.send_at_command(f'AT+CGDCONT={id},"IP","{apn}"')
        self.send_at_command('AT+CFUN=1')
        self.send_at_command('AT+CEREG=4')
        self.send_at_command('AT+QENG="servingcell"')
        self.send_at_command('AT+QNWINFO')
        self.send_at_command('AT+COPS?')
        self.send_at_command('AT+QIACT=1')
        self.send_at_command('AT+QIACT?')



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
        self.send_at_command(f'AT+QMTSNPUB={client_id},{msg_id},{qos},{retain},"{topic}",{msglen}')
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

