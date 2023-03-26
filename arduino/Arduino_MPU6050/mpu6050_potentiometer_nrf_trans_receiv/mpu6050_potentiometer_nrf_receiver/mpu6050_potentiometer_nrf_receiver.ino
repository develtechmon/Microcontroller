#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(4, 5); // CE, CSN
const byte address[6] = "00001";
char receivedData[32] = "";
int xAxis, yAxis, Zaxis;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setAutoAck(false);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(0,address);
  radio.startListening();
}

unsigned long last_Time =0;

void receive_the_data()
{
  if(radio.available())
  {
    byte Array[6];
    radio.read(&Array, sizeof(Array));
    Serial.print("Land L = ");
    Serial.print(Array[4]);
    Serial.print("  ");
    Serial.print("Guided G = ");
    Serial.print(Array[3]);
    Serial.print("  ");
    Serial.print("Throttle T = ");
    Serial.print(Array[2]);
    Serial.print("  ");
    Serial.print("Axis X = ");
    Serial.print(Array[0]);
    Serial.print("  ");
    Serial.print("Axis Y = ");
    Serial.println(Array[1]);
  }
}

void loop() {
  receive_the_data();
 }
