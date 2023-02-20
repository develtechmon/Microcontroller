//Add the servo library
#include <Servo.h>

//Add DC Motor library
#include <AFMotor.h>

//Define our servos
Servo servo1;

//Define DC motor
AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

//Define joystick pins (Analog)
int joyX = A8;
int joyY = A9;

//Variable to read the values from analog pins
int joyValx, joyValy;

//Declare pin state
int valx = 0;
int valy = 0;

//Declare all Led Pin
int ledPins[] = {3,4,5,6,7,8};
int thisPin = 0;
int pinCount = 6;

//Declare milis to record time
unsigned long previousTimeLed1 = 0;
long timeIntervalLed1 = 0;

//Define Motor speed
int mSpeed;

void setup()
{    
    Serial.begin(9600);
    pinMode(joyX, INPUT);
    pinMode(joyY, INPUT);

    for(thisPin = 0; thisPin < pinCount; thisPin++)
    {
        pinMode(ledPins[thisPin], OUTPUT);
    }
}

void loop()
{   
    unsigned long currentTime = millis();

    joyValx = analogRead(joyX);
    valx = map(joyValx, 0, 1023, 0, 254);

    //joyValy = analogRead(joyY);
    //valy = map(joyValy, 0, 1023, 0, 254);

    String Sp = " ; ";
    Serial.println(joyValx);
    //Serial.println(Sp);
    //Serial.println(valy);


  /*if (currentTime - previousTimeLed1 > timeIntervalLed1)
    {
        previousTimeLed1 = currentTime;
        for(thisPin = pinCount-1; thisPin>=0; thisPin--)
        {
            digitalWrite(ledPins[thisPin],HIGH);
            condition(joyValx);
            digitalWrite(ledPins[thisPin],LOW);
            condition(joyValx);
        }
    }*/
    
    //Release the motor
    if (valx >= 129 || valx <=130) 
    {
        Release();
    }
    
    //Move Forward 
    if (valx > 190)
    {   
        //y-y1=m(x-x1)
        mSpeed = (255./512.)*joyValx-255;
        Forward(mSpeed);

    }

    //Move Backward
    if (valx < 90)
    {   
        //y-y1=m(x-x1)
        mSpeed = (-255./512.)*joyValx+255;
        Backward(mSpeed);
    }            
}

void Release()
{
    motor1.run(RELEASE);
    motor1.setSpeed(0);
    motor2.run(RELEASE);
    motor2.setSpeed(0);
    motor3.run(RELEASE);
    motor3.setSpeed(0);
    motor4.run(RELEASE);
    motor4.setSpeed(0);
}

void Forward(int x)
{   
    motor1.run(FORWARD);
    motor1.setSpeed(x);
    motor2.run(FORWARD);
    motor2.setSpeed(x);
    motor3.run(FORWARD);
    motor3.setSpeed(x);
    motor4.run(FORWARD);
    motor4.setSpeed(x);
}

void Backward(int x)
{ 
    motor1.run(BACKWARD);
    motor1.setSpeed(x);
    motor2.run(BACKWARD);
    motor2.setSpeed(x);  
    motor3.run(BACKWARD);
    motor3.setSpeed(x);
    motor4.run(BACKWARD);
    motor4.setSpeed(x);
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

void Release()
{
    motor1.run(RELEASE);
    motor1.setSpeed(0);
    motor2.run(RELEASE);
    motor2.setSpeed(0);
    motor3.run(RELEASE);
    motor3.setSpeed(0);
    motor4.run(RELEASE);
    motor4.setSpeed(0);
}

void condition(int x)
{
Serial.println(x);
if (x == 0)
{
    timeIntervalLed1 = 36;
    //delay(36);
}

else 
{
    timeIntervalLed1 = x;
    //delay(x);
}
}


