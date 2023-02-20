
#include <Wire.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_DCMotor *Motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *Motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *Motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *Motor4 = AFMS.getMotor(4);

int defaultSpeed = 100;
int sidespeed = 170;

unsigned long eventInterval = 1000;
unsigned long previousTime1 = 0;
unsigned long previousTime2 = 0;

//--Ultrasonic Library--
#include <NewPing.h>

//--Define HC-Sro4 Pin--
#define trigPin 2
#define echoPin 3
#define MAX_DISTANCE 200

//--Define HC-Sro4 state--
NewPing sonar (trigPin, echoPin, MAX_DISTANCE);
int duration, distance;

void setup() {
   Serial.begin(9600);
  
   //Ultrasonic State
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  AFMS.begin();
  Motor1 -> setSpeed(defaultSpeed);
  Motor2 -> setSpeed(defaultSpeed);
  Motor3 -> setSpeed(defaultSpeed);
  Motor4 -> setSpeed(defaultSpeed);
}

void loop() {
distance = sonar.ping_cm();
Serial.println(distance);

 if (distance < 30)
 {
  Backward();
  delay(1000);
  RotateLeft();
  delay(1000);
  Forward();
  delay(500);
 }

 else {
  Forward();
 }

}

void Backward(){
  Motor1 -> run(FORWARD);
  Motor2 -> run(BACKWARD);
  Motor3 -> run(BACKWARD);
  Motor4 -> run(FORWARD); 
}

void Forward(){
  Motor1 -> run(BACKWARD);
  Motor2 -> run(FORWARD);
  Motor3 -> run(FORWARD);
  Motor4 -> run(BACKWARD); 
}

void Release(){
  Motor1 -> run(RELEASE);
  Motor2 -> run(RELEASE);
  Motor3 -> run(RELEASE);
  Motor4 -> run(RELEASE); 
}

void RotateLeft() {
  Motor1 -> run(FORWARD);
  Motor2 -> run(FORWARD);
  Motor3 -> run(FORWARD);
  Motor4 -> run(FORWARD); 
}

void RotateRight() {
  Motor1 -> run(BACKWARD);
  Motor2 -> run(BACKWARD);
  Motor3 -> run(BACKWARD);
  Motor4 -> run(BACKWARD); 
}

void SideLeft() {
  Motor1 -> setSpeed(sidespeed);
  Motor2 -> setSpeed(sidespeed);
  Motor3 -> setSpeed(sidespeed);
  Motor4 -> setSpeed(sidespeed);
  
  Motor1 -> run(FORWARD);
  Motor2 -> run(FORWARD);
  Motor3 -> run(BACKWARD);
  Motor4 -> run(BACKWARD); //f
}

void SideRight() {
  Motor1 -> setSpeed(sidespeed);
  Motor2 -> setSpeed(sidespeed);
  Motor3 -> setSpeed(sidespeed);
  Motor4 -> setSpeed(sidespeed);
  
  Motor1 -> run(BACKWARD);
  Motor2 -> run(BACKWARD);
  Motor3 -> run(FORWARD); 
  Motor4 -> run(FORWARD); //b
  
}
