from time import sleep
from SX127x.LoRa import *
from SX127x.board_config import BOARD
from firebase import firebase
firebase= firebase.FirebaseApplication('https://project-XXXXXXXXXXXXXXX.firebaseio.com', None) #paste your projeect url

BOARD.setup()

class LoRaRcvCont(LoRa):
    def __init__(self, verbose=False):
        super(LoRaRcvCont, self).__init__(verbose)
        self.set_mode(MODE.SLEEP)
        self.set_dio_mapping([0] * 6)

    def start(self):
        self.reset_ptr_rx()
        self.set_mode(MODE.RXCONT)
        while True:
            sleep(.5)
            rssi_value = self.get_rssi_value()
            status = self.get_modem_status()
            sys.stdout.flush()


    def on_rx_done(self):
        print("\nReceived: ")
        self.clear_irq_flags(RxDone=1)
        payload = self.read_payload(nocheck=True)
        msg = print(bytes(payload).decode("utf-8",'ignore'))
        sort = msg.splitlines()
        text = {
            'name':sort[0] ,
            'address':sort[1],
            'email':sort[2],
            'mob_number':sort[3],
            'clothing':sort[4],
            'food':sort[5],
            'med':sort[6],
            'power':sort[7]
        }
        print(msg)
        result = firebase.post('list/' , text)
        text = ''
        
        self.set_mode(MODE.SLEEP)
        self.reset_ptr_rx()
        self.set_mode(MODE.RXCONT)
lora = LoRaRcvCont(verbose=False)
lora.set_mode(MODE.STDBY)
lora.set_freq(433.0)
#  Medium Range  Defaults after init are 434.0MHz, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on 13 dBm

lora.set_pa_config(pa_select=1)

try:
    lora.start()
except KeyboardInterrupt:
    sys.stdout.flush()
    print("")
    sys.stderr.write("KeyboardInterrupt\n")
finally:
    sys.stdout.flush()
    print("")
    lora.set_mode(MODE.SLEEP)
    BOARD.teardown()

