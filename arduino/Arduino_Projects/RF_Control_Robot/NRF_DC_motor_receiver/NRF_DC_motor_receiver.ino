
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <AFMotor.h>

//Define DC motor
AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

//Define Motor speed
int mSpeed;

RF24 radio(36, 34); // CE, CSN
const byte address[6] = "00001";

struct Received_data {
  byte ch1;
  byte ch2;
};

Received_data received_data;

int ch1_value = 0;
int ch2_value = 0;

void setup() {

  received_data.ch1 = 130;
  received_data.ch2 = 130;
  
  Serial.begin(9600);
  radio.begin();
  radio.setAutoAck(false);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(0,address);
  radio.startListening();
  pinMode(22,OUTPUT);
  pinMode(48,OUTPUT);
  pinMode(35,OUTPUT);
  pinMode(31,OUTPUT);
}

unsigned long last_Time =0;

void receive_the_data()
{
  if(radio.available()> 0)
  {
    radio.read(&received_data, sizeof(Received_data));
    last_Time = millis();
  }
}

void loop() {
  receive_the_data();
  ch1_value = received_data.ch1;
  ch2_value = received_data.ch2;
  String sp1 = " : ";
  Serial.println(ch1_value + sp1 + ch2_value);

   //Release the motor
    if (ch1_value >=130)
    { 
        digitalWrite(22,HIGH);
        digitalWrite(48,HIGH);
        digitalWrite(35,LOW);
        digitalWrite(31,LOW);
        Release();
      
    }

    //Move Forward 
    if (ch1_value > 190)
    {   
        digitalWrite(35,HIGH);
        digitalWrite(31,HIGH);
        digitalWrite(22,LOW);
        digitalWrite(48,LOW);
        //y-y1=m(x-x1)
        mSpeed = (255./523.)*ch1_value-255;
        Forward(90);

    }

    //Move Backward
    if (ch1_value < 90)
    {   
        digitalWrite(22,LOW);
        digitalWrite(48,LOW);
        //y-y1=m(x-x1)
        mSpeed = (-255./516.)*ch1_value+255;
        Backward(90);
    }     

    //Move Left
    if (ch2_value < 90)
    {   
        digitalWrite(22,HIGH);
        digitalWrite(48,LOW);
        digitalWrite(35,LOW);
        digitalWrite(31,HIGH);
        Left(150);
    }     
    
    //Move Right
    if (ch2_value > 170)
    {   
        digitalWrite(22,LOW);
        digitalWrite(48,HIGH);
        digitalWrite(35,HIGH);
        digitalWrite(31,LOW);
        Right(150); 
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
