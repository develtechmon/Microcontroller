int vibr_Pin =A5;

int piezoPin = 8;

void setup(){
  //pinMode(LED_Pin, OUTPUT);
  pinMode(vibr_Pin, INPUT); //set vibr_Pin input for measurment
  Serial.begin(9600); //init serial 9600
 // Serial.println("----------------------Vibration demo------------------------");
}
void loop(){
  long measurement =TP_init();
  delay(50);
 // Serial.print("measurment = ");
  //Serial.println(measurement);
  if (measurement > 10000){
    //digitalWrite(LED_Pin, HIGH);
    tone(piezoPin, 440, 200);
  }
  else{
    //digitalWrite(LED_Pin, LOW); 
    noTone(8);
  }
}
long TP_init(){
  delay(10);
  long measurement=pulseIn (vibr_Pin, HIGH);  //wait for the pin to get HIGH and returns measurement
  return measurement;
}
