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
  Array[0] = 23;
  Array[1] = 34;
  
  radio.write(&Array, sizeof(Array)); // Send the message
  
  delay(1);
}
