#define ledtx 2
#define ledrx 3

int flag=0;
int state;

void setup ()
{   
    Serial1.begin(38400);
    Serial1.println("Start\n\n");
    pinMode(ledtx, OUTPUT);
    pinMode(ledrx, OUTPUT);
    
}

void loop()
{
    if (Serial1.available() > 0) 
    {
        state = Serial1.read();
        flag = 0;
    }

    if (state == '1') 
    {   
        digitalWrite(ledtx, LOW);
        digitalWrite(ledrx, LOW);
        if (flag==0){
            Serial1.println("Turn Off");
            flag=1;
        }
    }

    else if (state == '2')
    {
        digitalWrite(ledtx, HIGH);
        digitalWrite(ledrx, HIGH);
        if (flag==0){
            Serial1.println("Turn On");
            flag=1;
        }
    }
    
}