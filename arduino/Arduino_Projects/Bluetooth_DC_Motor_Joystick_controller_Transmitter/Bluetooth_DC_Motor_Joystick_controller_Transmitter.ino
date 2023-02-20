
//Define joystick pins (Analog)
int joyX = A5;
int joyY = A6;
int sw = 2;

//Variable to read the values from analog pins
int joyValx = 0;
int joyValy = 0;

//Declare pin state
int valx = 0;
int valy = 0;
int sw_state = 0;

//Space
String sp1 = " : ";

void setup()
{
    Serial.begin(38400);
    pinMode(joyX, INPUT);
    pinMode(joyY, INPUT);
    pinMode(sw, INPUT_PULLUP); 
}

void loop()
{   
        valx =analogRead(joyX);
        valy =analogRead(joyY);
        sw_state = digitalRead(sw);

        joyValx = map(valx, 0, 1023, 0, 2);
        joyValy = map(valy, 0, 1023, 0, 3);
        //delay(100);
        //Serial.println(joyValx + sp1+  joyValy + sp1 + sw_state + sp1 + valx + sp1 + valy);

        switch(joyValx) {
            case 1:
            Serial.println('R');
            break;

            case 2:
            Serial.println('F');
            break;

            case 0:
            Serial.println('B');
            break;

        }
        delay(100);





        //Release
        /*if (joyValx ==130)
        {
            Serial.println('R');
        }

        //Move Forward 
        if (joyValx > 190)
        {   
            //y-y1=m(x-x1)
            //mSpeed = (255./512.)*joyValx-255;
            //Forward(mSpeed);
            Serial.println('F');
        }

        //Move Backward
        if (joyValx < 90)
        {   
            //y-y1=m(x-x1)
            //mSpeed = (-255./516.)*joyValx+255;
            //Backward(100);
            Serial.println('B');
        } */

        //String sp1 = " ; ";
        //Serial.println(joyValx + sp1+  joyValy + sp1 + sw_state + sp1 + valx + sp1 + valy);

}
