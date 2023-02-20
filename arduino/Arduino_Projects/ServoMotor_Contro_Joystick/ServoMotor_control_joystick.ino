//Add the servo library
#include <Servo.h>

//Define our servos
Servo servo1;

//Define joystick pins (Analog)
int joyX = 0;
int joyY = 1;
int initial_position = 90;

//Variable to read the values from analog pins
int joyValx, joyValy;

void setup()
{    
    Serial.begin(9600);
    servo1.attach(3);
    servo1.write(initial_position);
    pinMode(joyX, INPUT);
    pinMode(joyValy, INPUT);
}

void loop()
{   
    
    joyValx = analogRead(joyX);
    delay(15);
    Serial.println(joyValx);
    joyValx = map(joyValx, 0, 1023, 0, 179);
    servo1.write(joyValx);
    delay(15);

    joyValy = analogRead(joyY);
    delay(15);
    Serial.println(joyValx);
    joyValy = map(joyValy, 0, 1023, 0, 179);
    servo1.write(joyValy);
}


