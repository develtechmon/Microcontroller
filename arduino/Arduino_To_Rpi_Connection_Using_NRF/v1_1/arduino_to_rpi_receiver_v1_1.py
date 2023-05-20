from __future__ import print_function
import time
from RF24 import RF24, RF24_PA_LOW
import RPi.GPIO as GPIO

pipes = [0xF0F0F0F0E1, 0xF0F0F0F0D2]

radio = RF24(22, 0);
radio.begin()
radio.setAutoAck(False);
radio.enableDynamicPayloads()
radio.setRetries(5,15)
radio.printDetails()

print('Role: Pong Back, awaiting transmission')
radio.openWritingPipe(pipes[1])
radio.openReadingPipe(1,pipes[0])
radio.startListening()

def read_data():
    if radio.available():
        while radio.available():
            data = radio.getDynamicPayloadSize()
            receive_payload = radio.read(data)
            receive_payload.decode('utf-8')

            # READ - First, stop listening so we can talk
            print(receive_payload)
            #print("Sent the message: {}".format(receive_payload))
            #radio.stopListening()

            # WRITE - Second, Send the final one back.
            #radio.write(receive_payload)
            #print('Got response.')

            # Now, resume listening so we catch the next packets.
            #radio.startListening()
            time.sleep(0.5)
    
if __name__ == "__main__":
    while True:
        read_data()
