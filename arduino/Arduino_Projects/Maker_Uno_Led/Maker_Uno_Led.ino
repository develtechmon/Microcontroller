int pin;

void setup() {
  Serial.begin(9600);
  for (pin =2; pin <14; pin++)
  {
    pinMode(pin,OUTPUT);
  }
 
}

void loop() {
  for (pin = 2; pin <14; pin++)
  {
    digitalWrite(pin, HIGH);
    delay(100);
  }
  
  
  for (pin = 2; pin <14; pin++)
  {
    digitalWrite(pin, LOW);
  }
    
}
