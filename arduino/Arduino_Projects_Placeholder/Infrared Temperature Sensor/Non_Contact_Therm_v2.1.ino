/*
from GuyThatDoesEverything:
youtube.com/GuyThatDoesEverything
This source code has tons of room for improvements
as it was written in quite short time.
Feel free to contact me for any help/queries:
arsalanstien@gmail.com
*/

#include <Wire.h>
#include <SparkFun_APDS9960.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);




SparkFun_APDS9960 apds = SparkFun_APDS9960();
uint8_t proximity_data = 0;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {

  mlx.begin();
  apds.init();
  apds.enableProximitySensor(false);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  Serial.begin(9600);

}


void loop() {
  String temperature = "";
  /*
    Serial.print("Ambient ");
    Serial.print(mlx.readAmbientTempC());
    Serial.print(" C");
    Serial.println();

    Serial.print("Target  ");
    Serial.print(mlx.readObjectTempC());
    Serial.print(" C");
    Serial.println();
  */
  apds.readProximity(proximity_data);


  if (proximity_data == 255 && mlx.readObjectTempF() < 100) {


    temperature = String(mlx.readObjectTempC(), 1);
    Serial.print(" Body Temperature:");
    Serial.println(mlx.readObjectTempC());
    display.clearDisplay();
    display.invertDisplay(false);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(8, 0);
    display.clearDisplay();
    display.println("Body Temp:");
    display.setCursor(25, 18);
    display.print(mlx.readObjectTempF());

    display.setCursor(85, 8);
    display.print(".");


    display.setTextColor(WHITE);
    display.setCursor(85, 18);
    display.print(" F");
    display.display();







    delay(1000);

  }
  if (proximity_data == 255) {
    if (mlx.readObjectTempF() > 102) {
      noTone(5);
      // play a note on pin 8 for 500 ms:
      tone(8, 523, 500);

      display.clearDisplay();
      display.invertDisplay(false);
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(20, 0);
      display.clearDisplay();
      display.println("CRITICAL");

      display.invertDisplay(true);
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(20, 0);
      display.clearDisplay();
      display.println("CRITICAL");


      display.invertDisplay(true);
      display.setTextColor(WHITE);
      display.setCursor(20, 0);
      display.clearDisplay();
      display.println("CRITICAL");


      display.invertDisplay(false);
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(20, 0);
      display.clearDisplay();
      display.println("CRITICAL");

      display.invertDisplay(true);
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(20, 0);
      display.clearDisplay();
      display.println("CRITICAL");


      display.invertDisplay(true);
      display.setTextColor(WHITE);
      display.setCursor(20, 0);
      display.clearDisplay();
      display.println("CRITICAL");


      display.invertDisplay(false);
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(20, 0);
      display.clearDisplay();
      display.println("CRITICAL");




      display.setCursor(23, 18);
      display.print(mlx.readObjectTempF());
      display.setCursor(93, 8);
      display.print(".");
      display.setTextColor(WHITE);
      display.setCursor(93, 18);
      display.print(" F");
      display.display();


      delay(1000);
    }

  }

  if (proximity_data == 255) {
    if (mlx.readObjectTempF() > 100) {

      display.setCursor(93, 8);
      display.print(".");


      display.setTextColor(WHITE);
      display.setCursor(93, 18);
      display.print(" F");
      display.display();



    }
  }


  else if (proximity_data <= 255) {
    delay(1000);

    display.clearDisplay();
    display.invertDisplay(true);
    display.setTextSize(2.8);
    display.setTextColor(WHITE);
    display.setCursor(45, 1);
    display.clearDisplay();
    display.println("NOT");
    display.setCursor(17, 17);
    display.print("DETECTED");
    display.display();
    digitalWrite(5, LOW);

  }



}

