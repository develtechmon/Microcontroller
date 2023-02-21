#define BLYNK_TEMPLATE_ID "TMPL0gUMkq36"
#define BLYNK_DEVICE_NAME "ESP8266"
#define BLYNK_AUTH_TOKEN "1RlX0mIgDMKAtMvx3fmL4Sh73WCKa3ju"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <Wire.h>              // Wire library (required for I2C devices)
#include <Adafruit_BMP280.h>   // Adafruit BMP280 sensor library

// define device I2C address: 0x76 or 0x77 (0x77 is library default address)
#define BMP280_I2C_ADDRESS  0x76

Adafruit_BMP280  bmp;

// Go to the Project Settings (nut icon).
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "WLAN1-0762EH";
char pass[] = "Lukas@92";

BlynkTimer timer;

void bmpSensor()
{
  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");
   
  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");
   
  Serial.print("Approx altitude = ");
  Serial.print(bmp.readAltitude(1013.25)); // this should be adjusted to your local forcase
  Serial.println(" m");
   
  Serial.println();
  //delay(2000);
  
  Blynk.virtualWrite(V0, bmp.readTemperature());
  Blynk.virtualWrite(V1, bmp.readPressure());
  Blynk.virtualWrite(V2, bmp.readAltitude(1013.25));
  
}

void setup() {
  Serial.begin(115200);
   Serial.println(F("BMP280 test"));

  if ( !bmp.begin(BMP280_I2C_ADDRESS))
  {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
   while (1);
  }
 
  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  
  Blynk.begin(auth, ssid, pass);
  
  timer.setInterval(1000L, bmpSensor);

}

void loop() {
  Blynk.run();
  timer.run(); 
}
