
/*ARDUINO LINE FOLLOWING CAR
 * Created by Glen Tech 
 * https://www.youtube.com/glentech
  */
  
#include <AFMotor.h>

//defining pins and variables
#define rights A0
#define lefts A1
#define middle A2 

//defining motors
AF_DCMotor motor1(1, MOTOR12_1KHZ); 
AF_DCMotor motor2(2, MOTOR12_1KHZ);
//AF_DCMotor motor3(3, MOTOR34_1KHZ);
//AF_DCMotor motor4(4, MOTOR34_1KHZ);



void setup() {

  //declaring pin types
  pinMode(lefts,INPUT);
  pinMode(rights,INPUT);
  pinMode(middle,INPUT);
  //begin serial communication
  Serial.begin(115200);
  
}

void loop(){
  //printing values of the sensors to the serial monitor
  //Serial.println(analogRead(lefts));
  //Serial.println(analogRead(rights));
  //line detected by both
  if(analogRead(lefts)<=350 && analogRead(rights)<=350 && analogRead(middle)<=350 ){
    //Forward
    motor1.run(FORWARD);
    motor1.setSpeed(90);
    motor2.run(FORWARD);
    motor2.setSpeed(90);

    //motor3.run(FORWARD);
    //motor4.run(FORWARD);
  }
  //line detected by left sensor
  else if(analogRead(lefts)<=350 && !analogRead(rights)<=350){
    //turn left
    motor1.run(FORWARD);
    motor1.setSpeed(200);
    motor2.run(BACKWARD);
    motor2.setSpeed(200);
    //motor3.run(BACKWARD);
    //motor4.run(BACKWARD);
    
  }
  //line detected by right sensor
  else if(!analogRead(lefts)<=350 && analogRead(rights)<=350){
    //turn right
    motor1.run(BACKWARD);
    motor2.setSpeed(200);
    motor2.run(FORWARD);
    motor2.setSpeed(200);
    //motor3.run(FORWARD);
    //motor4.run(FORWARD);
   
  }
  //line detected by none
  else if(!analogRead(lefts)<=350 && !analogRead(rights)<=350){
    //stop
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    //motor3.run(RELEASE);
    //motor4.run(RELEASE);
   
  }
  
}
