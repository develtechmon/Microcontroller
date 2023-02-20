#include <SoftwareSerial.h>

//SoftwareSerial BTSerial(2,3);

void setup()
{
    Serial.begin(38400);
    //BTSerial.begin(38400);
}

void loop()
{
       
        //Serial.write('1');
        Serial.println('1');
        delay(1000);

       // Serial.write('2');
        Serial.println('2');
        delay(1000);   
}
