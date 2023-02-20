#define led1 13
#define led2 12
#define piezoPin 8

char state = 0;

void setup()
{
    Serial.begin(9600);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
}
 
void loop()
{   
    tone(piezoPin, 1000,1000);
    Serial.println("High");
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    delay(1000);

    Serial.println("Low");
    digitalWrite(piezoPin, LOW);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    delay(1000);
}