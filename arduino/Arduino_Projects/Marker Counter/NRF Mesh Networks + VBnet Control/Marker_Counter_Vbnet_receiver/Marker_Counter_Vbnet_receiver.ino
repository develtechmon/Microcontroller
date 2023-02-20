/*
:Marker Counter vbnet receiver
:Author: Lukas Johnny
:Email: Lukasjohnny2328@gmail.
:Marker Vbnet v1.0
:Date: 19/08/2020
:Revision: v1.0
:License: TechMon
*/


//Include Necessary Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);

//Declare nRF pin and const address
RF24 radio(2, 7); // CE, CSN
const byte address[][6] =  {"00001", "00002"};

struct Received_data {
  byte ch1;
};

Received_data received_data;

int ch1_value = 0;
int ch2_value = 0;
int count = 0;
int marks = 0;
int piezoPin = 8;
byte byteRead;
int port_com_state = 0;
String sp1 = " : ";

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(false);
  radio.openReadingPipe(1, address[0]); //Set the address at which the data will be send
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
  
   //Detecting the Arduino Port COM automatically
   if (Serial.available() ) {
    byteRead = Serial.read();

    switch (byteRead) {  
    case 't':
      Serial.write("OK");
      port_com_state = 1;
      test();
      return;
      break;
      
      default:
      break;
    }
  }
 }
  /*radio.startListening();
  lcd.setCursor(3,0);
  lcd.print("Hits Count");
  receive_the_data();
  ch1_value = received_data.ch1;

  if(ch1_value == 0) {
    lcd.setCursor(7,1);
    lcd.print(" "); 
    lcd.setCursor(8,1);
    lcd.print(" "); 
  }
  //ch2_value = received_data.ch2;
  //Serial.println(ch1_value + sp1 + ch2_value);

   lcd.setCursor(7,1);
   lcd.print(ch1_value);
   String sp1 = "Hits Count ";

   Serial.println(ch1_value);
 }*/
  
  void test()
  {
    if (port_com_state == 1) {
     for (int i = 0; i < 100; i++) {
    Serial.println(i);
    delay(1000);
    }
   
  }
  }
