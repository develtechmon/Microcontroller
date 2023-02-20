
#include <AFMotor.h>

//defining motor pins and variable
#define left A0
#define right A1

//defining motors
AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

int distance;
long duration;

void setup() {
  pinMode(left, INPUT);
  pinMode(right, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
}

void loop() {

  //Serial.println(digitalRead(left));
 // Serial.println(digitalRead(right));


 
 if (digitalRead(left) == 0 && digitalRead(right) == 0) 
  {
    Forward();
  }

 if (digitalRead(left) == 1 && digitalRead(right) == 1) 
 {
  Backward();
  delay(800);
  Right();
  delay(590);
  Forward();
  delay(200);  
 }

  if (digitalRead(left) == 0 && digitalRead(right) == 1) 
  {
  Backward();
  delay(800);
  Right();
  delay(590);
  Forward();
  delay(200);  
 }

  if (digitalRead(left) == 1 && digitalRead(right) == 0) 
  {
  Backward();
  delay(800);
  Left();
  delay(590);
  Forward();
  delay(200);  
 }

}

void Forward()
{
    motor1.run(FORWARD);
    motor1.setSpeed(90);
    motor2.run(FORWARD);
    motor2.setSpeed(90);
    motor3.run(FORWARD);
    motor3.setSpeed(90);
    motor4.run(FORWARD);
    motor4.setSpeed(90);
}

void Release()
{
    motor1.run(RELEASE);
    motor1.setSpeed(90);
    motor2.run(RELEASE);
    motor2.setSpeed(90);
    motor3.run(RELEASE);
    motor3.setSpeed(90);
    motor4.run(RELEASE);
    motor4.setSpeed(90);
}

void Backward()
{
    motor1.run(BACKWARD);
    motor1.setSpeed(90);
    motor2.run(BACKWARD);
    motor2.setSpeed(90);
    motor3.run(BACKWARD);
    motor3.setSpeed(90);
    motor4.run(BACKWARD);
    motor4.setSpeed(90);
  
}

void Right()
{
    motor1.run(FORWARD);
    motor1.setSpeed(90);
    motor2.run(BACKWARD);
    motor2.setSpeed(90);
    motor3.run(BACKWARD);
    motor3.setSpeed(90);
    motor4.run(FORWARD);
    motor4.setSpeed(90);
  
}

void Left()
{
    motor1.run(BACKWARD);
    motor1.setSpeed(90);
    motor2.run(FORWARD);
    motor2.setSpeed(90);
    motor3.run(FORWARD);
    motor3.setSpeed(90);
    motor4.run(BACKWARD);
    motor4.setSpeed(90);
}
