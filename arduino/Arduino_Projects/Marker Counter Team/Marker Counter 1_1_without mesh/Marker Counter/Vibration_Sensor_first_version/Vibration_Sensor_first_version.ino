
int piezoPin = 8;

int vibration_Sensor = A5;
int present_condition = 0;
int previous_condition = 0;


void setup() {
  Serial.begin(9600);
  pinMode(vibration_Sensor, INPUT);
 }

void loop() {
  previous_condition = present_condition;
  present_condition = digitalRead(vibration_Sensor);
  delay(100);
  if (previous_condition != present_condition) {
    led_blink();
    Serial.println(present_condition);

    } 

  else {
    //digitalWrite(LED, OFF);
     noTone(8);
    }
}

void led_blink() {
  tone(piezoPin, 440, 200);
  delay(100);
  /*digitalWrite(LED, ON);
  delay(250);
  digitalWrite(LED, OFF);
  delay(250);
  digitalWrite(LED, ON);
  delay(250);
  digitalWrite(LED, OFF);
  delay(250);*/
}

 /* sens = digitalRead(2);
  Serial.println(sens);

  if (sens == 1) {
    tone(piezoPin, 440, 200);
    delay(300);
   }
  else {
    noTone(8);
  }*/
