void setup() {
  Serial.begin(9600);
}

void loop() {
  int rawValue = analogRead(A0);

  Serial.println(rawValue);

  delay(100);
}
