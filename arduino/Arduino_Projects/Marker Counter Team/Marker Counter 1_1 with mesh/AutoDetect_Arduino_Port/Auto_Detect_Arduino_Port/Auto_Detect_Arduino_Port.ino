byte byteRead;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    byteRead = Serial.read();
    
    switch (byteRead) {  
    case 't':
      Serial.println("OK");
      return;
      break;

      default:
      break;
    }
  }
}
