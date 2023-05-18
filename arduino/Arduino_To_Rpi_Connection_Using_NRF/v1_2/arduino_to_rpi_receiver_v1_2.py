'''
In this v1_2, we're going to use different nrf24 library. Instructions as follow
1. Open this Link alongside for your reference
2. git clone https://github.com/BLavery/lib_nrf24.git
'''

from lib_nrf24 import NRF24  
import RPi.GPIO as GPIO  
import spidev

GPIO.setmode(GPIO.BCM)

pipes = [0xF0F0F0F0E1, 0xF0F0F0F0D2]

radio = NRF24(GPIO, spidev.SpiDev())
radio.begin(0, 22)   
radio.setAutoAck(True)       
radio.enableDynamicPayloads()

#radio.setPayloadSize(32) 
radio.setChannel(115)
#radio.setDataRate(NRF24.BR_1MBPS)   
#radio.setPALevel(NRF24.PA_MIN) 
radio.enableAckPayload()

radio.openReadingPipe(1,pipes[0])
radio.startListening()
radio.printDetails()    

def read_data():
    if radio.available():
        while radio.available():
            x = []
            len = radio.getDynamicPayloadSize()
            receive_payload = radio.read(x,len)
            #receive_payload.decode('utf-8')
            
            # READ - First, stop listening so we can talk
            print(receive_payload)
            #print("Sent the message: {}".format(receive_payload))
            radio.stopListening()

            # WRITE - Second, Send the final one back.
            #radio.write(receive_payload)
            print('Got response.')

            # Now, resume listening so we catch the next packets.
            #radio.startListening()

if __name__ == "__main__":
    while True:
        read_data()
