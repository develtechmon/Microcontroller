int vibr_Pin_A =D0;

void setup() {

  Serial.begin(9600); // Starts the serial communication
  pinMode(vibr_Pin_A, INPUT);

}
void loop() {
    long measurement_A =TP_init_A();
    Serial.println(measurement_A);
}

long TP_init_A(){
  delay(10);
  long measurement_A=pulseIn (vibr_Pin_A, HIGH);  //wait for the pin to get HIGH and returns measurement_A
  return measurement_A;
}
