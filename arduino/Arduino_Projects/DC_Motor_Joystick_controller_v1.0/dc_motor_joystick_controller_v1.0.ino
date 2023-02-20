//Add DC Motor library
#include <AFMotor.h>

//Define DC motor
AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

//Define joystick pins (Analog)
int joyX = A15;
int joyY = A9;
int sw = 53;

//Variable to read the values from analog pins
int joyValx = 0;
int joyValy = 0;

//Declare pin state
int valx = 0;
int valy = 0;
int sw_state = 0;

//Declare all Led Pin
int ledPins[] = {3,4,5,6,7,8};
int thisPin = 0;
int pinCount = 6;

//Declare milis to record time
unsigned long currentTime = 0;

unsigned long previousTimeX = 0;
unsigned long previousTimeY = 0;

const int valXinterval = 100;
const int valYinterval = 110;

//Define Motor speed
int mSpeed;

void setup()
{    
    Serial.begin(9600);
    pinMode(joyX, INPUT);
    pinMode(joyY, INPUT);
    pinMode(sw, INPUT_PULLUP); 
}

void loop()
{   
    valx =analogRead(joyX);
    valy =analogRead(joyY);
    sw_state = digitalRead(sw);

    joyValx = map(valx, 0, 1023, 0, 255);
    joyValy = map(valy, 0, 1023, 0, 255);
    
    String sp1 = " ; ";
    Serial.println(joyValx + sp1+  joyValy + sp1 + sw_state + sp1 + valx + sp1 + valy);
    delay(100);

    //Release the motor
    if (joyValx >=130 && joyValy <=128)
    {
        Release();
    }

    //Move Forward 
    if (joyValx > 190)
    {   
        //y-y1=m(x-x1)
        mSpeed = (255./523.)*joyValx-255;
        Forward(90);

    }

    //Move Backward
    if (joyValx < 90)
    {   
        //y-y1=m(x-x1)
        mSpeed = (-255./516.)*joyValx+255;
        Backward(90);
    }     

    //Move Left
    if (joyValy < 90)
    {
        Left(100);
    }     
    
    //Move Right
    if (joyValy > 170)
    {
        Right(100);
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

void Right(int x)
{
    motor1.run(FORWARD);
    motor1.setSpeed(x);
    motor2.run(BACKWARD);
    motor2.setSpeed(x);
    motor3.run(BACKWARD);
    motor3.setSpeed(x);
    motor4.run(FORWARD);
    motor4.setSpeed(x);
}

void Left(int x)
{
    motor1.run(BACKWARD);
    motor1.setSpeed(x);
    motor2.run(FORWARD);
    motor2.setSpeed(x);
    motor3.run(FORWARD);
    motor3.setSpeed(x);
    motor4.run(BACKWARD);
    motor4.setSpeed(x);
}
