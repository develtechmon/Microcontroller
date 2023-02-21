//Author Lukas Johnnyy
//Visit My Facebook Lukas Johnny


//--------For Ultrasonic----------
#include <NewPing.h>
//-----------For Servo Motor----------
#include <Servo.h>

//Define Ultrasonic pin
#define trigPin 2
#define echoPin 3
#define stop_distance 200
int ballFound = 0;
int basketFound = 0;

NewPing sonar(trigPin, echoPin, stop_distance);
int distance;
long duration;

int servo_1 = 0;
int servo_2 = 0;
int servo_3 = 0;

//Define Servo
Servo servo1;
Servo servo2;
Servo servo3;

//Pin declaration
int RightEn = 5;
int RightDir = 4;
int LeftEn = 6;
int LeftDir = 7;
int LeftSen = A1;
int LeftMSen = A2;
int MidSen = A3;
int RightMSen = A4;
int RightSen = A5;

void setup()
{
  //Serial.begin(115200);
  //Motor Driver Pin Setup
  pinMode(RightEn,OUTPUT);
  pinMode(RightDir,OUTPUT);
  pinMode(LeftEn,OUTPUT);
  pinMode(LeftDir,OUTPUT);
  
  digitalWrite(RightDir,HIGH);
  digitalWrite(LeftDir,LOW);
  
  //LSS05 Auto-Calibrating Line Sensor Pin Setup
  pinMode(LeftSen,INPUT);
  pinMode(LeftMSen,INPUT);
  pinMode(MidSen,INPUT);
  pinMode(RightMSen,INPUT);
  pinMode(RightSen,INPUT);

  //Setup for servo motors
  servo1.attach(8);
  servo2.attach(10);
  servo3.attach(11);

  //Setup for initial position of the servos
  servo1.write(40); //gripper 
  servo2.write(40); //gripper supporter
  servo3.write(90); //stand for servo

  //Declare Ultrasonic state
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop()
{  
  if((digitalRead(LeftSen)==0)&&(digitalRead(LeftMSen)==0)&&(digitalRead(MidSen)==1)&&(digitalRead(RightMSen)==0)&&(digitalRead(RightSen)==0))
  {
    analogWrite(RightEn, 200);   //PWM Speed Control
    analogWrite(LeftEn, 200);   //PWM Speed Control
  }
  else if((digitalRead(LeftSen)==0)&&(digitalRead(LeftMSen)==1)&&(digitalRead(MidSen)==1)&&(digitalRead(RightMSen)==0)&&(digitalRead(RightSen)==0))
  {
    analogWrite(RightEn, 200);   //PWM Speed Control
    analogWrite(LeftEn, 140);   //PWM Speed Control
  }
  else if((digitalRead(LeftSen)==0)&&(digitalRead(LeftMSen)==1)&&(digitalRead(MidSen)==0)&&(digitalRead(RightMSen)==0)&&(digitalRead(RightSen)==0))
  {
    analogWrite(RightEn, 200);   //PWM Speed Control
    analogWrite(LeftEn, 90);   //PWM Speed Control
  }
  else if((digitalRead(LeftSen)==1)&&(digitalRead(LeftMSen)==1)&&(digitalRead(MidSen)==0)&&(digitalRead(RightMSen)==0)&&(digitalRead(RightSen)==0))
  {
    analogWrite(RightEn, 200);   //PWM Speed Control
    analogWrite(LeftEn, 50);   //PWM Speed Control
  }
  else if((digitalRead(LeftSen)==1)&&(digitalRead(LeftMSen)==0)&&(digitalRead(MidSen)==0)&&(digitalRead(RightMSen)==0)&&(digitalRead(RightSen)==0))
  {
    analogWrite(RightEn, 200);   //PWM Speed Control
    analogWrite(LeftEn, 0);   //PWM Speed Control
  }
  else if((digitalRead(LeftSen)==0)&&(digitalRead(LeftMSen)==0)&&(digitalRead(MidSen)==1)&&(digitalRead(RightMSen)==1)&&(digitalRead(RightSen)==0))
  {
    analogWrite(RightEn, 140);   //PWM Speed Control
    analogWrite(LeftEn, 200);   //PWM Speed Control
  }
  else if((digitalRead(LeftSen)==0)&&(digitalRead(LeftMSen)==0)&&(digitalRead(MidSen)==0)&&(digitalRead(RightMSen)==1)&&(digitalRead(RightSen)==0))
  {
    analogWrite(RightEn, 90);   //PWM Speed Control
    analogWrite(LeftEn, 200);   //PWM Speed Control
  }
  else if((digitalRead(LeftSen)==0)&&(digitalRead(LeftMSen)==0)&&(digitalRead(MidSen)==0)&&(digitalRead(RightMSen)==1)&&(digitalRead(RightSen)==1))
  {
    analogWrite(RightEn, 50);   //PWM Speed Control
    analogWrite(LeftEn, 200);   //PWM Speed Control
  }
  else if((digitalRead(LeftSen)==0)&&(digitalRead(LeftMSen)==0)&&(digitalRead(MidSen)==0)&&(digitalRead(RightMSen)==0)&&(digitalRead(RightSen)==1))
  {
    analogWrite(RightEn, 0);   //PWM Speed Control
    analogWrite(LeftEn, 200);   //PWM Speed Control
  }
  detectobject();
  //speedtest();
}


void detectobject()
{
 Serial.print("Ping: ");
 distance = sonar.ping_cm();
 Serial.println(sonar.ping_cm()); 
 Serial.println("cm");

  if (distance > 2 && distance < 10 && ballFound == 0 && basketFound == 0)
 {
    analogWrite(RightEn, 0);   //PWM Speed Control
    analogWrite(LeftEn, 0);   //PWM Speed Control
    
  //Ball Found, let's catch the ball !!
  delay(1000);
  if (servo_3 == 0 && servo_2 == 0 && servo_1 == 0)
    {

   //Servo 3 will go down to assist the position of servo that control gripper motor
     for (int i = 90; i>20; i--)
    {
      servo3.write(i);
      delay(50);
      //servo_3 = 1;
      //servo_2 = 1;
    }  
    
   //Servo 2 going down to assist gripper
    for (int i = 40; i<100; i++)
  {
    servo2.write(i);
    //servo_1 = 1;
    //servo_2 = 0;
    //servo_3 = 0;
    delay(50);
  }  

   //Gripper going down and pick the ball
   delay(1000);
   for (int i = 40; i<160; i++)
  {
    servo1.write(i);
    delay(30);
  }
  delay(1000);
  ballFound = 1;
  basketFound = 0;
  }
 //Gripper will hold the ball and Servo 2 will going up  
    delay(100);
    for (int i = 100; i>15; i--)
    {
      servo1.write(160);
      servo2.write(i);
      delay(50);
    }
    
     //Long servo will go up
    for (int i = 30; i<90; i++)
    {
    servo3.write(i);
    delay(50);
 
    ballFound = 1;
    basketFound = 1;
    servo_3 = 1;
    servo_1 = 1;
    servo_2 = 1;
   }  
 
  }
//--------------------------

// Thhis line for fucntions when Robot detect a Basket and drop the balls
 else if (distance > 2 && distance < 10 && basketFound == 1 && ballFound == 1)
 {
 Serial.println("Putting the ball into basket !");
   analogWrite(RightEn, 0);   //PWM Speed Control
   analogWrite(LeftEn, 0);   //PWM Speed Control
    
  delay(1000);
   //Servo 3 will go down to assist the position of servo that control gripper motor
     if (servo_3 == 1 && servo_2 == 1 && servo_1 == 1)
    {
     for (int i = 90; i>30; i--)
    {
      servo3.write(i);
      delay(50);
      //servo_3 = 1;
      //servo_2 = 1;
    }  

    delay(1000);
    //Servo 2 going down to assist gripper
    for (int i = 15; i<100; i++)
  {
    servo2.write(i);
    //servo_1 = 1;
    //servo_2 = 0;
    //servo_3 = 0;
    delay(50);
  }
  
  //Gripper going down and drop the ball
   delay(1000);
   for (int i = 160; i>0; i--)
  {
    servo1.write(i);
    delay(30);
  }
  delay(1000);
  }

  //Gripper will drop the ball and Servo 2 will going up  
    delay(100);
    for (int i = 100; i>15; i--)
    {
      //servo1.write(160);
      servo2.write(i);
      delay(50);
    }

     //Long servo will go up
    for (int i = 30; i<90; i++)
    {
    servo3.write(i);
    delay(50);
    ballFound = 0;
    basketFound = 0;
    servo_3 = 0;
    servo_1 = 0;
    servo_2 = 0;  
   }
}
}
