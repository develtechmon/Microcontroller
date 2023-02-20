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

RF24 radio(9, 8); // CE, CSN
const byte address[6] = "00001";

struct MyData {
  byte X;
  byte Y;
};

MyData data;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
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

  data.X = map(ax, -17000, 17000, 0, 255 ); //Send X axis data
  data.Y = map(ay, -17000, 17000, 0, 255);  //Send Y axis data

  delay(50);
  radio.write(&data, sizeof(MyData));

  Serial.print("Axis X = ");
  Serial.print(data.X);
  Serial.print("  ");
  Serial.print("Axis X = ");
  Serial.println(data.X);
}
