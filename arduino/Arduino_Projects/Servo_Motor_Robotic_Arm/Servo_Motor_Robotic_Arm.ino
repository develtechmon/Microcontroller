#include <Servo.h>

int servo_1 = 0;
int servo_2 = 0;
int servo_3 = 0;

//Define Servo
Servo servo1;
Servo servo2;
Servo servo3;

void setup(){
  Serial.begin(115200); // Initialize serial with 9600bps
  
  //Setup for servo motors
  servo1.attach(8);
  servo2.attach(10);
  servo3.attach(11);

  //Setup for initial position of the servos
  servo1.write(40); //gripper 
  servo2.write(40); //gripper supporter
  servo3.write(90); //stand for servo
}

void loop(){
    if (servo_3 == 0 && servo_2 == 0 && servo_1 == 0)
    {

   // Servo 3 will go down to assist the position of servo that control gripper motor
     for (int i = 90; i>9; i--)
    {
      servo3.write(i);
      delay(50);
      servo_3 = 1;
      servo_2 = 1;
    }
    }

  //Servo 2 going down to assist gripper
  if (servo_1 == 0 && servo_2 == 1 && servo_3 == 1)
  {
     for (int i = 40; i<125; i++)
  {
    servo2.write(i);
    servo_1 = 1;
    servo_2 = 0;
    servo_3 = 0;
    delay(50);
  }  
  }
 
  //Gripper going down and pick the ball
  if (servo_1 == 1 && servo_2 == 0 && servo_3 == 0)
  {
   delay(1000);
   for (int i = 40; i<140; i++)
  {
    servo1.write(i);
    delay(30);
  }
  delay(1000);
  servo_2 = 1;


  }

  //Gripper will hold the ball and Servo 2 will going up
  if (servo_2 == 1 && servo_1 == 1 && servo_3 == 0)
  {
    servo1.write(140);
    delay(100);
    for (int i = 125; i>40; i--)
    {
      servo2.write(i);
      delay(50);
    }
    //servo_1 = 1;
    //servo_2 = 0;
    servo_3 = 1;
    servo_2 = 0;
  }

  //Long servo will go up
  if (servo_2 == 0 && servo_3 == 1 && servo_1 == 1)
  {
    for (int i = 9; i<90; i++)
    {
    servo3.write(i);
    delay(50);
    servo_3 = 0;
    servo_1 = 0;
    servo_2 = 1;  
   } 
  }
   


}

  //servo1.write(0);
  //delay(1000);
 /* servo2.write(130);
  servo3.write(30);
  delay(1000);
  ballFound = 1;
  basketFound = 0;
  Serial.println("YEah found the ball and catch it !");
  delay(1000);
  servo1.write(90);
  delay(1000);
  servo2.write(130);
  servo3.write(30);
  delay(1000);*/
    
  
