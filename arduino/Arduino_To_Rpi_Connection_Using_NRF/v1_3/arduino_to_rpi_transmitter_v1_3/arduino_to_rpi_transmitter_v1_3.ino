#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Pico W
//RF24 radio(7, 8); // Set CE and CSN pins

// Arduino Nano
RF24 radio(4, 5); // Set CE and CSN pins

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

byte Array[2];

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.enableDynamicPayloads();

  radio.setRetries(5, 15);
  radio.openWritingPipe(pipes[0]); // Set the address to send data
  radio.setPALevel(RF24_PA_MIN); // Set power level to high - orig RF24_PA_HIGH
  radio.stopListening();
}

void loop() {
  //Array[0] = 23;
  Array[1] = 34;

    if (Serial.available() > 0) {
    // read incoming serial data:
    char inChar = Serial.read();

    if (inChar == 'g') {
      Serial.println("Enter Guided Mode");
      Array[0] = 0;
    }

    else if (inChar == 'h') {
      Serial.println("Enter Stabilized Mode");
      Array[0] = 1;
    }

    else if (inChar == 'u') {
      Serial.println("Enter Takeoff Mode");
      Array[0] = 2;
    }

    else if (inChar == 'd') {
      Serial.println("Go Right");
      Array[0] = 3;
    }

    else if (inChar == 'a') {
      Serial.println("Go Left");
      Array[0] = 4;
    }

    else if (inChar == 's') {
      Serial.println("Go Back");
      Array[0] = 5;
    }

    else if (inChar == 'w') {
      Serial.println("Go Forward");
      Array[0] = 6;
    }

    else if (inChar == 'l') {
      Serial.println("Enter Land Mode");
      Array[0] = 7;
    }

    else if (inChar == 'h') {
      Serial.println("Enter Hover Mode");
      Array[0] = 8;
    }

    else if (inChar == 'q') {
      Serial.println("Yaw Left");
      Array[0] = 9;
    }

    else if (inChar == 'e') {
      Serial.println("Yaw Right");
      Array[0] = 'e';
    }
  
  Serial.println(Array[0]);

  radio.write(&Array, sizeof(Array)); // Send the message
  
  delay(1);
}
}
