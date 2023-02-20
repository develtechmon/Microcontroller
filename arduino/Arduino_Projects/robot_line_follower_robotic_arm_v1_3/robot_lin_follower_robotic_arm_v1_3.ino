/*
:Author: Lukas Johnny
:Email: Lukasjohnny2328@gmail.
:Robot_Line_Follower_Robotic_Arm_Ultrasonic v1.3
:Date: 23/07/2020
:Revision: version1.3
:License: TechMon
*/

//--Ultrasonic Library--
#include <NewPing.h>

//--Servo Library--
#include <Servo.h>

//--Servo Motor--
Servo servo1;
Servo servo2;
Servo servo3;

//--Define HC-Sro4 Pin--
#define trigPin 2
#define echoPin 3
#define MAX_DISTANCE 350

//--Define HC-Sro4 state--
NewPing sonar (trigPin, echoPin, MAX_DISTANCE);
float duration, distance;

//--Define Flag--
int ballFound = 0;
int basketFound = 0;

int servo_1 = 0;
int servo_2 = 0;
int servo_3 = 0;

//--Define Motor Pin--
int RightEn = 5;
int RightDir = 4;
int LeftEn = 6;
int LeftDir = 7;

//--Define LS05 Motor Pin--
int LeftSen = A1;
int LeftMSen = A2;
int MidSen = A3;
int RightMSen = A4;
int RightSen = A5;

void setup() {
  //Motor Driver Pin Setup
  pinMode(RightEn, OUTPUT);
  pinMode(RightDir, OUTPUT);
  pinMode(LeftEn, OUTPUT);
  pinMode(LeftDir, OUTPUT);
  
  //LSS05 Pin Setup
  pinMode(LeftSen, INPUT);
  pinMode(LeftMSen, INPUT);
  pinMode(MidSen, INPUT);
  pinMode(RightMSen, INPUT);
  pinMode(RightSen, INPUT);
  
  servo1.attach(8);
  servo2.attach(10);
  servo3.attach(11);
  
  //Initial Setup position of Servo Motors
  servo1.write(40); //gripper
  servo2.write(40); //gripper supporter
  servo3.write(90); //stand for servo
  
  //Ultrasonic State
  pinMode(trigPin, OUTPUT);
  pinMode(trigPin, INPUT);
}

void loop() {
  
  following_line();
  delay(500);
  object_detection();
  delay(500);
}

void following_line()
{ 
  //Motor Direction Forward
  digitalWrite(RightDir, HIGH);
  digitalWrite(LeftDir, LOW);
  
  //LSS05 will read the dark stripe and move based on its specified PWM value
  if((digitalRead(LeftSen)==0)&&(digitalRead(LeftMSen)==0)&&(digitalRead(MidSen)==1)&&(digitalRead(RightMSen)==0)&&(digitalRead(RightSen)==0))
  {
    analogWrite(RightEn, 40);   //PWM Speed Control //200
    analogWrite(LeftEn, 40);   //PWM Speed Control //200
  }
  else if((digitalRead(LeftSen)==0)&&(digitalRead(LeftMSen)==1)&&(digitalRead(MidSen)==1)&&(digitalRead(RightMSen)==0)&&(digitalRead(RightSen)==0))
  {
    analogWrite(RightEn, 40);   //PWM Speed Control //200
    analogWrite(LeftEn, 30);   //PWM Speed Control //140
  }
  else if((digitalRead(LeftSen)==0)&&(digitalRead(LeftMSen)==1)&&(digitalRead(MidSen)==0)&&(digitalRead(RightMSen)==0)&&(digitalRead(RightSen)==0))
  {
    analogWrite(RightEn, 40);   //PWM Speed Control //200
    analogWrite(LeftEn, 20);   //PWM Speed Control  //90
  }
  else if((digitalRead(LeftSen)==1)&&(digitalRead(LeftMSen)==1)&&(digitalRead(MidSen)==0)&&(digitalRead(RightMSen)==0)&&(digitalRead(RightSen)==0))
  {
    analogWrite(RightEn, 40);   //PWM Speed Control //200
    analogWrite(LeftEn, 10);   //PWM Speed Control  //50
  }
  else if((digitalRead(LeftSen)==1)&&(digitalRead(LeftMSen)==0)&&(digitalRead(MidSen)==0)&&(digitalRead(RightMSen)==0)&&(digitalRead(RightSen)==0))
  {
    analogWrite(RightEn, 40);   //PWM Speed Control //200
    analogWrite(LeftEn, 0);   //PWM Speed Control //0
  }
  else if((digitalRead(LeftSen)==0)&&(digitalRead(LeftMSen)==0)&&(digitalRead(MidSen)==1)&&(digitalRead(RightMSen)==1)&&(digitalRead(RightSen)==0))
  {
    analogWrite(RightEn, 30);   //PWM Speed Control //140
    analogWrite(LeftEn, 40);   //PWM Speed Control  //200
  }
  else if((digitalRead(LeftSen)==0)&&(digitalRead(LeftMSen)==0)&&(digitalRead(MidSen)==0)&&(digitalRead(RightMSen)==1)&&(digitalRead(RightSen)==0))
  {
    analogWrite(RightEn, 20);   //PWM Speed Control //90
    analogWrite(LeftEn, 40);   //PWM Speed Control //200
  }
  else if((digitalRead(LeftSen)==0)&&(digitalRead(LeftMSen)==0)&&(digitalRead(MidSen)==0)&&(digitalRead(RightMSen)==1)&&(digitalRead(RightSen)==1))
  {
    analogWrite(RightEn, 10);   //PWM Speed Control //50
    analogWrite(LeftEn, 40);   //PWM Speed Control  //200
  }
  else if((digitalRead(LeftSen)==0)&&(digitalRead(LeftMSen)==0)&&(digitalRead(MidSen)==0)&&(digitalRead(RightMSen)==0)&&(digitalRead(RightSen)==1))
  {
    analogWrite(RightEn, 0);   //PWM Speed Control //0
    analogWrite(LeftEn, 40);   //PWM Speed Control //200
  }
}

void object_detection()
{
  delay(50);
  duration = sonar.ping();
  distance = (duration/2) * 0.0343;
  Serial.print("Distance = ");
  Serial.print(distance);
  
  if ((distance > 3)&&(distance < 15)&&(ballFound==0)&&(basketFound==0))
  {
    //Ball Found, let's catch the ball !!
    if (servo_3 == 0 && servo_2 == 0 && servo_1 == 0)
    { 
      analogWrite(RightEn,0);
      analogWrite(LeftEn,0);
      delay(1000);   
      
      //Servo 3 will go from 90 position to its defined degree position that control the gripper motor
      for (int i = 90; i > 30; i--) {
        servo3.write(i);
        delay(50);
      }
      
      //Servo 2 will go down to assist the gripper
      for (int i = 40; i < 100; i++) {
        servo2.write(i);
        delay(50);
      }
      delay(1000);
      
      //Servo 1 Gripper going down and pick the ball
      for (int i = 40; i < 160; i++) {
        servo1.write(i);
        delay(50);
      }
      delay(1000);
      
      //Servo 1 Gripper will hold the ball and Servo 2 will go up
      for (int i = 100; i > 15; i--) { // ------ Consider to change to 40 later
        servo1.write(160); // Gripper will hold the motor
        servo2.write(i);
        delay(50);
      }
      
      //Servo 3 will go up
      for (int i = 30; i<90; i++)
      {
        servo3.write(i);
        delay(50);
        if ((i == 89)||(i == 90))
        {
          ballFound = 1;
          basketFound = 1;
        }
      }
      if ((ballFound == 1)&&(basketFound == 1)) {
        servo_3 = 1;
        servo_2 = 1;
        servo_1 = 1;
        //Motor Direction Right
        digitalWrite(RightDir, LOW);
        digitalWrite(LeftDir, LOW);
        analogWrite(RightEn, 120);   //PWM Speed Control
        analogWrite(LeftEn, 120);   //PWM Speed Control
        delay(1000);
        
        if((digitalRead(LeftSen)==1) || (digitalRead(LeftMSen)==1) || (digitalRead(MidSen)==1)|| (digitalRead(RightMSen)==1)|| (digitalRead(RightSen)==1))
        {
          analogWrite(RightEn,0);
          analogWrite(LeftEn,0);
          delay(500);
          following_line();
        }
    }
  }
  
  else if ((distance > 3)&&(distance < 15)&&(ballFound==1)&&(basketFound==1)) {
    
    //Basket Found, let's drop the ball !!
    if ((servo_3 == 1)&&(servo_2 == 1)&&(servo_1 == 1)) 
    { 
      analogWrite(RightEn, 0);
      analogWrite(LeftEn, 0);
      delay(1000);
      
      //Servo 3 going down again
      for (int i = 90; i > 30; i--) {
        servo3.write(i);
        delay(50);
      }    
      delay(1000);
      
      //Servo 2 will go down to assist the gripper
      for (int i = 15; i<100; i++) {
        servo2.write(i);
        delay(50);
      }
      delay(1000);
  
      //Servo 1 Gripper go down and drop the  ball
      for (int i = 160; i>0; i--) {
        servo1.write(i);
        delay(50);
      }
      delay(1000);
      
      //Servo 2 will go up
      for (int i = 100; i>15; i--) {
        servo2.write(i);
        delay(50);
      }
      
      //Servo 3 going up again
      for (int i = 30; i<90; i++) {
        servo3.write(i);
        delay(50);
        if ((i == 89)||(i == 90))
        {
          ballFound = 0;
          basketFound = 0;
        }
      }  
      if ((ballFound == 0)&&(basketFound == 0)) {
        servo_3 = 0;
        servo_2 = 0;
        servo_1 = 0;
        
        //Motor Direction Right
        digitalWrite(RightDir, LOW);
        digitalWrite(LeftDir, LOW);
        analogWrite(RightEn, 120);
        analogWrite(LeftEn, 120);
        delay(1000);
        
        if((digitalRead(LeftSen)==1) || (digitalRead(LeftMSen)==1) || (digitalRead(MidSen)==1)|| (digitalRead(RightMSen)==1)|| (digitalRead(RightSen)==1))
        {
          analogWrite(RightEn,0);
          analogWrite(LeftEn,0);
          delay(500);
          following_line();
        }
      }
    }      
  } 
  }
}
