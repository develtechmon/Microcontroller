
#include <AFMotor.h>

//defining motors
//AF_DCMotor motor1(1, MOTOR12_1KHZ);
//AF_DCMotor motor2(4, MOTOR34_1KHZ);

AF_DCMotor motor1(1, MOTOR12_8KHZ); 
AF_DCMotor motor2(2, MOTOR12_8KHZ);

void setup() {

  Serial.begin(9600);
}

void loop() {
 
  Backward();
  delay(1000);
  //Right();
  //delay(1000);
  Forward();
  delay(1000);
  //Left();
  //delay(1000);
 
}

void Forward()
{
    motor1.run(FORWARD);
    motor1.setSpeed(90);

    motor2.run(FORWARD);
    motor2.setSpeed(90);
 
}

void Release()
{
    motor1.run(RELEASE);
    motor1.setSpeed(90);

    motor2.run(RELEASE);
    motor2.setSpeed(90);
}

void Backward()
{
    motor1.run(BACKWARD);
    motor1.setSpeed(90);

    motor2.run(BACKWARD);
    motor2.setSpeed(90);
}

void Right()
{
    motor1.run(FORWARD);
    motor1.setSpeed(90);

    motor2.run(BACKWARD);
    motor2.setSpeed(90);
}

void Left()
{
    motor1.run(BACKWARD);
    motor1.setSpeed(90);

    motor2.run(FORWARD);
    motor2.setSpeed(90);
}
