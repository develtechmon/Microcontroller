#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // Set CE and CSN pins
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

byte Array[2];

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.enableDynamicPayloads();

  radio.setRetries(5, 15);
  radio.openWritingPipe(pipes[0]); // Set the address to send data
  radio.setPALevel(RF24_PA_HIGH); // Set power level to high
  radio.stopListening();
}

void loop() {
  //Array[0] = 23;
  //Array[1] = 34;

    if (Serial.available() > 0) {
    // read incoming serial data:
    char inChar = Serial.read();

    if (inChar == 'g') {
      Serial.println("Enter Guided Mode");
      Array[0] = 'g';
      
    }

    else if (inChar == 'h') {
      Serial.println("Enter Stabilized Mode");
      Array[0] = 'h';
    }

    else if (inChar == 'u') {
      Serial.println("Enter Takeoff Mode");
      Array[0] = 'u';
    }

    else if (inChar == 'd') {
      Serial.println("Go Right");
      Array[0] = 'd';
    }

    else if (inChar == 'a') {
      Serial.println("Go Left");
      Array[0] = 'a';
    }

    else if (inChar == 's') {
      Serial.println("Go Back");
      Array[0] = 's';
    }

    else if (inChar == 'w') {
      Serial.println("Go Forward");
      Array[0] = 'w';
    }

    else if (inChar == 'l') {
      Serial.println("Enter Land Mode");
      Array[0] = 'l';
    }

    else if (inChar == 'h') {
      Serial.println("Enter Hover Mode");
      Array[0] = 'h';
    }

    else if (inChar == 'q') {
      Serial.println("Yaw Left");
      Array[0] = 'q';
    }

    else if (inChar == 'e') {
      Serial.println("Yaw Right");
      Array[0] = 'e';
    }
  
  radio.write(&Array, sizeof(Array)); // Send the message
  
  delay(1);
}
}
