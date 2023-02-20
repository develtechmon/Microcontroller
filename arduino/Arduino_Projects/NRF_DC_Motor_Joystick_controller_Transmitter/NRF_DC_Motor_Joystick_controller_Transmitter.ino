#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//Define joystick pins (Analog)
#define joyX = A5;
#define joyY = A6;
int sw = 2;

//Declare pin state
String valx, valy;
char xyData[32] = "";
int sw_state = 0;

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {

valx =analogRead(A5);
valy =analogRead(A6);
sw_state = digitalRead(sw);


// X value
valx.toCharArray(xyData, 5);
radio.write(&xyData, sizeof(xyData));

// Y value
valy.toCharArray(xyData, 5);
radio.write(&xyData, sizeof(xyData));

delay(20);

}

