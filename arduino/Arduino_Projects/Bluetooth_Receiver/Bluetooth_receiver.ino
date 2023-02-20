#include <SoftwareSerial.h>
#define piezoPin 8

SoftwareSerial BTSerial(2,3);

char incoming;

void setup()
{
    Serial.begin(38400);
    //BTSerial.begin(38400);
    pinMode(6, OUTPUT);
}

void loop()
{  
   if (Serial.available()>0)
   {
       incoming=Serial.read();
   }

    if (incoming == '1')
    {    
        String p1 ="-->";
        Serial.println(p1 + incoming);
        tone(piezoPin, 1000,1000);
        digitalWrite(6, HIGH);
    }

    if (incoming == '2')
    {   
        String p1 ="-->";
        Serial.println(p1 + incoming);
        digitalWrite(6,LOW);
    }
}
