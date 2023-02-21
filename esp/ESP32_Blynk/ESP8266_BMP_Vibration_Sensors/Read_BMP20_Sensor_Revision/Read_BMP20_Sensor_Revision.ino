
#include <Wire.h>              // Wire library (required for I2C devices)
#include <Adafruit_BMP280.h>   // Adafruit BMP280 sensor library

// define device I2C address: 0x76 or 0x77 (0x77 is library default address)
#define BMP280_I2C_ADDRESS  0x76
// initialize Adafruit BMP280 library

Adafruit_BMP280  bmp;


void setup() {
  Serial.begin(9600);
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
}

void loop() {

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
delay(2000);

}
