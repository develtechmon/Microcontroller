#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <AFMotor.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);

//Define DC motor
AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

//Define Motor speed
int mSpeed;

RF24 radio(36, 34);
const byte address[6] = "00001";

struct Received_data {
  byte X;
  byte Y;
};

Received_data received_data;

void setup()
{
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  radio.begin();
  radio.setAutoAck(false);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(0,address);
  radio.startListening();

}

void recvData()
{
  if ( radio.available() ) {
    radio.read(&received_data, sizeof(Received_data));


    if (received_data.Y < 80) { //Reverse
         lcd.setCursor(3,0);
        lcd.print("Reverse");
        digitalWrite(22,LOW);
        digitalWrite(48,LOW);
        //y-y1=m(x-x1)
        //mSpeed = (-255./516.)*ch1_value+255;
        Backward(90);

    }

    if (received_data.Y > 145) {//forward
        lcd.setCursor(3,0);
        lcd.print("Forward");
        digitalWrite(35,HIGH);
        digitalWrite(31,HIGH);
        digitalWrite(22,LOW);
        digitalWrite(48,LOW);
        //y-y1=m(x-x1)
        //mSpeed = (255./523.)*ch1_value-255;
        Forward(90);
    }

    if (received_data.X > 155) {//right turn
          lcd.setCursor(3,0);
        lcd.print("<-Right");
        digitalWrite(22,LOW);
        digitalWrite(48,HIGH);
        digitalWrite(35,HIGH);
        digitalWrite(31,LOW);
        Right(130); 
    }

    if (received_data.X < 80) {//left turn
     lcd.setCursor(3,0);
        lcd.print("-->Left");
        digitalWrite(22,HIGH);
        digitalWrite(48,LOW);
        Left(130);
    }

    if (received_data.X > 100 && received_data.X < 170 && received_data.Y > 80 && received_data.Y < 130) { //stop car
         lcd.setCursor(3,0);
        lcd.print("Release");
        digitalWrite(22,HIGH);
        digitalWrite(48,HIGH);
        digitalWrite(35,LOW);
        digitalWrite(31,LOW);
        Release();
    }
  }
}

void loop()
{
  recvData();
  Serial.print("X: ");
  Serial.print(received_data.X);
  Serial.print("    ");
  Serial.print("Y: ");
  Serial.print(received_data.Y);
  Serial.print("\n");
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
 
