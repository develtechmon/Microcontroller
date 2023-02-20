//Default arduino micro internal led
#define ledpinrx 17
#define ledpintx 30

//Define the data set from Bluetooth
int state = 0;

void setup() {

  Serial1.begin(9600);
  pinMode(ledpinrx, OUTPUT);
  pinMode(ledpintx, OUTPUT);
  digitalWrite(ledpinrx, LOW);
  digitalWrite(ledpintx, LOW);
}

void loop() {


    digitalWrite(ledpinrx,HIGH);
    digitalWrite(ledpintx,HIGH);
    delay(1000);
    digitalWrite(ledpinrx,LOW);
    digitalWrite(ledpintx,LOW);
    delay(1000);
 
}
