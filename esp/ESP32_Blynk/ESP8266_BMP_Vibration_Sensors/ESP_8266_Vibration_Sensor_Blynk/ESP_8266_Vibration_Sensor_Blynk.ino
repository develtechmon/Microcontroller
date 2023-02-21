#define BLYNK_TEMPLATE_ID "TMPLdV1FWyoH"
#define BLYNK_DEVICE_NAME "Pressure Sensor"
#define BLYNK_AUTH_TOKEN "Rmw8AGBuSxKQzT_PZKtyN50m8FGVV3ig"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Go to the Project Settings (nut icon).
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "WLAN1-0762EH";
char pass[] = "Lukas@92";

BlynkTimer timer;

int vibr_Pin_A =D0;

void pressureSensor()
{
  long measurement_A =TP_init_A();
  Serial.println(measurement_A);

  Blynk.virtualWrite(V5, measurement_A);
}

void setup() {
  Serial.begin(115200);
  pinMode(vibr_Pin_A, INPUT);
  
  Blynk.begin(auth, ssid, pass);
  
  timer.setInterval(1000L, pressureSensor);

}

void loop() {
  Blynk.run();
  timer.run(); 
}

long TP_init_A(){
  delay(10);
  long measurement_A=pulseIn (vibr_Pin_A, HIGH);  //wait for the pin to get HIGH and returns measurement_A
  return measurement_A;
}
