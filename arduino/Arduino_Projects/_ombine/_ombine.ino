#include <Servo.h>
#include <NewPing.h>

//Define Ultrasonic pin
#define trigPin 2
#define echoPin 3
#define stop_distance 200

int ballFound = 0;
int basketFound = 0;

NewPing sonar(trigPin, echoPin, stop_distance);

int distance;
long duration;

//Define Motor Pin
#define EN1   5 // Pin 5 is connected to EN1 (SHIELD_2AMOTOR)
#define DIR1  4 // Pin 4 is connected to DIR1 (SHIELD_2AMOTOR)
#define EN2   6 // Pin 6 is connected to EN2 (SHIELD_2AMOTOR)
#define DIR2  7 // Pin 7 is connected to DIR2 (SHIELD_2AMOTOR)

const int MAX_PWM = 255;
int sensor=0;
int lastSensor=0;


//Define IR Sensor Pin
#define LSS05_O1  A1 // Pin A1 is connected to O1 (LSS05)
#define LSS05_O2  A2 // Pin A2 is connected to O2 (LSS05)
#define LSS05_O3  A3 // Pin A3 is connected to O3 (LSS05)
#define LSS05_O4  A4 // Pin A4 is connected to O4 (LSS05)
#define LSS05_O5  A5 // Pin A5 is connected to O5 (LSS05)
#define LSS05_CAL 8 // Pin 8 is connected to CAL (LSS05)

//Define Servo
Servo servo1;
Servo servo2;
Servo servo3;


void setup(){
  Serial.begin(115200); // Initialize serial with 9600bps

  //Declare Ultrasonic state
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Setup for servo motors
  servo1.attach(8);
  servo2.attach(10);
  servo3.attach(12);

  //Setup for initial position of the servos
  servo1.write(90);
  servo2.write(30);
  servo3.write(120);

  //Declare motor State
  pinMode(EN1, OUTPUT); // Set EN1 as output
  pinMode(DIR1, OUTPUT); // Set DIR1 as output
  pinMode(EN2, OUTPUT); // Set EN2 as output
  pinMode(DIR2, OUTPUT); // Set DIR2 as output

  //Declare IR State
  pinMode(LSS05_O1, INPUT); // Set O1 as input
  pinMode(LSS05_O2, INPUT); // Set O2 as input
  pinMode(LSS05_O3, INPUT); // Set O3 as input
  pinMode(LSS05_O4, INPUT); // Set O4 as input
  pinMode(LSS05_O5, INPUT); // Set O5 as input
  pinMode(LSS05_CAL, OUTPUT); // Set CAL as output
  digitalWrite(LSS05_CAL, HIGH); // Set CAL to HIGH, default state for C

  lastSensor = sensor;

}

void loop(){
  pathfinder();
}

void pathfinder() {
  sensor = digitalRead(LSS05_O5) + (digitalRead(LSS05_O4) * 2) +
  (digitalRead(LSS05_O3) * 4) + (digitalRead(LSS05_O2) * 8) +
  (digitalRead(LSS05_O1) * 16);

   //Forward
  if (sensor == 0b01110 || sensor == 0b00100|| sensor == 0b11111) {
    motor(200,200);
    lastSensor = sensor;
  }

  //slightly right
  else if (sensor == 0b01100|| sensor == 0b01000 || sensor == 0b11100){
    motor(120, 200);
    lastSensor = sensor;
  }
  //Slightly Right
  else if (sensor == 0b11000){
    motor(60, 200);
    lastSensor = sensor;
  }
  else if (sensor == 0b10000){
    motor(0, 200);
    lastSensor = sensor;
  }
  else if(sensor == 0b00110 || sensor == 0b00010 || sensor == 0b00111){
    motor(200, 120);
    lastSensor = sensor;
  }
  else if (sensor == 0b00011){
    motor(200,60);
    lastSensor = sensor;
  }
  else if (sensor == 0b00001){
    motor(200, 0);
    lastSensor = sensor;
  }
  else if (sensor == 0b11110){
    motor(160, 200);
    lastSensor = sensor;
  }
  else if (sensor == 0b01111){
    motor(200, 160);
    lastSensor = sensor;
  }
  else if (sensor == 0b00000 && lastSensor == 0b01111){
    motor(200,0);
    lastSensor = sensor;
  }
  else if (sensor == 0b00000 && lastSensor == 0b00111){
    motor(200,0);
    lastSensor = sensor;
  }
  else if (sensor == 0b00000 && lastSensor == 0b00011){
    motor(200,0);
    lastSensor = sensor;
  }
  else if (sensor == 0b00000 && lastSensor == 0b11110){
    motor(0,200);
    lastSensor = sensor;
  }
  else if (sensor == 0b00000 && lastSensor == 0b11100){
    motor(0,200);
    lastSensor = sensor;
  }
  else if (sensor == 0b00000 && lastSensor == 0b11000){
    motor(0,200);
    lastSensor = sensor;
  }
  else if (sensor == 0b00000 && lastSensor == 0b01000){
    motor(0,200);
    lastSensor = sensor;
  }
  else if (sensor == 0b00000 && lastSensor == 0b00010){
    motor(200,0);
    lastSensor = sensor;
  }
  else if (sensor == 0b00000 && lastSensor == 0b00100){
    motor(0,200);
    lastSensor = sensor;
  }
    //detectobject();
}

void motor(signed int speedLeft,signed int speedRight){
  speedLeft = speedLeft*2;
  speedLeft = constrain(speedLeft,-MAX_PWM, MAX_PWM);

  speedRight = speedRight*2;
  speedRight = constrain(speedRight,-MAX_PWM,MAX_PWM);

  if (speedLeft > 0){
    digitalWrite(DIR2, LOW);
    analogWrite(EN2, speedLeft);
  }
  else {
    digitalWrite(DIR2, HIGH);
    analogWrite(EN2, abs(speedLeft));
  }

  if (speedRight > 0) {
    digitalWrite(DIR1, LOW);
    analogWrite(EN1, speedRight);
  }
  else {
    digitalWrite(DIR1, HIGH);
    analogWrite(EN1, abs(speedRight));
  }
}

void detectobject()
{
 Serial.print("Ping: ");
 distance = sonar.ping_cm();
 Serial.print(sonar.ping_cm()); 
 Serial.println("cm");

 if (distance <= 7 && ballFound == 0 && basketFound == 0)
 {
   motor(0,0);

  //Ball Found, let's catch the ball !!
  delay(1000);
  servo1.write(180);
  delay(1000);
  servo2.write(130);
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
  delay(1000);
  ballFound = 1;
  basketFound = 1;
  Serial.println("Hold still the ball !");


 }

  else if ((distance <=7) && (basketFound == 1) && (ballFound == 1))
 {
  motor(0,0);
  delay(1000);
  servo1.write(180);
  delay(1000);
  servo2.write(30);
  servo3.write(120);
  delay(1000);
  //ballFound = 0;
  //basketFound = 0;
  Serial.println("Putting the ball into basket !");
  


 }
 
}

    
  
