import time
from RF24 import RF24, RF24_PA_HIGH
import struct

pipes = [0xF0F0F0F0E1, 0xF0F0F0F0D2]

radio = RF24(22, 0)  # Set CE and CSN pins

radio.begin()
radio.openWritingPipe(pipes[1])  # Set the writing pipe
radio.openReadingPipe(1, pipes[0])  # Set the reading pipe
radio.setPALevel(RF24_PA_HIGH)  # Set power level to high

radio.setAutoAck(False);
radio.enableDynamicPayloads()
radio.setRetries(5,15)
radio.startListening()

while True:
    if radio.available():
        x = radio.read(radio.getDynamicPayloadSize())  # Read the received message
        x.decode('utf-8')
        print(x[0], x[1])
 