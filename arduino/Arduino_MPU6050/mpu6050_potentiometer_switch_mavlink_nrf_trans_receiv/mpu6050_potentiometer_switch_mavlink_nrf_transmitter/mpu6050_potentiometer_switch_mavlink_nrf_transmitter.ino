#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Wire.h"       
#include "I2Cdev.h"     
#include "MPU6050.h"    

//Define variables for Gyroscope and Accelerometer data
MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;

//Declare pin state
byte Array[6];
#define arm 2
#define disarm 3

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

void setup()
{
  Serial.begin(57600);
  Wire.begin();

  pinMode(land, INPUT_PULLUP);
  pinMode(guided, INPUT_PULLUP);
  
  mpu.initialize();

  radio.begin();
  radio.setAutoAck(false);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop()
{
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  Array[0] = digitalRead(arm);
  Array[1] = digitalRead(disarm);
  Array[2] = analogRead(A0);
  Array[3] = map(ax, -17000, 17000, 0, 255 ); //Send X axis data
  Array[4] = map(ay, -17000, 17000, 0, 255);  //Send Y axis data

  radio.write(&Array, sizeof(Array));

  Serial.print("Arm A = ");
  Serial.print(Array[0]);
  Serial.print("  ");

  Serial.print("Disarm D = ");
  Serial.print(Array[1]);
  Serial.print("  ");

  Serial.print("Throttle T = ");
  Serial.print(Array[2]);
  SSerial.print("  ");

  Serial.print("Roll R = ");
  Serial.print(Array[3]);
  Serial.print("  ");

  Serial.print("Pitch P = ");
  Serial.println(Array[4]);
 
  delay(50);
}
