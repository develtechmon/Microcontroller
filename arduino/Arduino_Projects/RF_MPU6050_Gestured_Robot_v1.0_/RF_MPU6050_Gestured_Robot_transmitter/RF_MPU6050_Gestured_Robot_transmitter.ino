#include <SPI.h>
#include <Wire.h>

#include <nRF24L01.h>
#include <RF24.h>
#define MPU 0x68 // I2C address of the MPU-6050

//Declare pin state
String valx, valy;
char xyData[32] = "";
int sw_state = 0;

RF24 radio(9, 8); // CE, CSN
const byte address[6] = "00001";

//Create a struct that do not exceed 32 bytes
struct Data_to_be_sent {
  byte ch1;
  byte ch2;
};

Data_to_be_sent sent_data;

double AcX, AcY, AcZ;
int Pitch, Roll;

void setup () {
  Serial.begin (9600);
  radio.begin();
  radio.setAutoAck(false);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(address);
  radio.stopListening(); 
  init_MPU (); // Initialization MPU6050
}
 
void loop ()
{
  FunctionsMPU (); // I acquire AcX, AcY, AcZ axes.
    
  Roll = FunctionsPitchRoll (AcX, AcY, AcZ); // Roll angle calculation
  Pitch = FunctionsPitchRoll (AcY, AcX, AcZ); // Pitch angle calculation

  int ServoRoll = map (Roll, -90, 90, 0, 179);
  int ServoPitch = map (Pitch, -90, 90, 0, 179);

  sent_data.ch1 = ServoRoll;
  sent_data.ch2 = ServoPitch;

  String pitch = " p :";
  String roll = " r :";
  
  radio.write(&sent_data, sizeof(Data_to_be_sent));

  Serial.println(pitch + sent_data.ch1 + roll + sent_data.ch2);
}

void init_MPU () {
  Wire.begin ();
  Wire.beginTransmission (MPU);
  Wire.write (0x6B); // PWR_MGMT_1 register
  Wire.write (0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission (true);
  delay (1000);
}

// Function for calculating the pitch and roll angles
double FunctionsPitchRoll (double A, double B, double C) {
  double DataA, DataB, Value;
  DataA = A;
  DataB = (B * B) + (C * C);
  DataB = sqrt (DataB);
  
  Value = atan2 (DataA, DataB);
  Value = Value * 180 / 3.14;
  
  return (int) Value;
}

// Function for acquiring the X, Y, Z axes of the MPU6050
void FunctionsMPU () {
  Wire.beginTransmission (MPU);
  Wire.write (0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission (false);
  Wire.requestFrom (MPU, 6, true); // request a total of 14 registers
  AcX = Wire.read () << 8 | Wire.read (); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY = Wire.read () << 8 | Wire.read (); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read () << 8 | Wire.read (); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
}
