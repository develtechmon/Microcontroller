#include <SPI.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
   lcd.setCursor(4,0);
   lcd.print("Counter");

   for (int i=0; i<10; i++) {
    lcd.setCursor(7,1);
    lcd.print(i);
    delay(1000);
   }

    
}
