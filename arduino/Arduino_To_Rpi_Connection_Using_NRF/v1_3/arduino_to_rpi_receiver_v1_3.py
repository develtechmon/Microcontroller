'''
Work flow
1. To read accelerometer value from arduino
2. Use this accelerometer to define the drone
direction
3. Add dronekit code
'''

import time
from RF24 import RF24, RF24_PA_MIN
import struct

pipes = [0xF0F0F0F0E1, 0xF0F0F0F0D2]

radio = RF24(22, 0)  # Set CE and CSN pins

radio.begin()
radio.openWritingPipe(pipes[1])  # Set the writing pipe
radio.openReadingPipe(1, pipes[0])  # Set the reading pipe
radio.setPALevel(RF24_PA_MIN)  # Set power level to high

radio.setAutoAck(False);
radio.enableDynamicPayloads()
radio.setRetries(5,15)
radio.startListening()

while True:
    if radio.available():
        x = radio.read(radio.getDynamicPayloadSize())  # Read the received message
        x.decode('utf-8')
        #print(x[0], x[1])
        print(x[0])
        
        # if x[0] == "g":
        #     print("Enter Guided Mode")
            
        # elif x[0] == "h":
        #     print("Enter Stabilized Mode")
            
        # elif x[0] == "u":
        #     print("Enter Takeoff Mode")
        
        # elif x[0] == "d":
        #     print("Go Right")
        
        # elif x[0] == "a":
        #     print("Go Left")
        
        # elif x[0] == "s":
        #     print("Go Back")
        
        # elif x[0] == "w":
        #     print("Go Forward")
        
        # elif x[0] == "l":
        #     print("Enter Land Mode")
        
        # elif x[0] == "h":
        #     print("Enter Hover Mode")
            
        # elif x[0] == "q":
        #     print("Yaw Left")
        
        # elif x[0] == "e":
        #     print("Yaw Right")
        
 