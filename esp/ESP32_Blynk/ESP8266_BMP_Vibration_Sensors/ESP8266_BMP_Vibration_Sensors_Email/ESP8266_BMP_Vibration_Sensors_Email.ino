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

#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <ESP_Mail_Client.h>

#define WIFI_SSID "WLAN1-0762EH"
#define WIFI_PASSWORD "Lukas@92"

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

/* The sign in credentials */
#define AUTHOR_EMAIL "sam98fyp@gmail.com"
#define AUTHOR_PASSWORD "ESP8266@Sam"

/* Recipient's email*/
//#define RECIPIENT_EMAIL "bearzsam98@gmail.com"
#define RECIPIENT_EMAIL "lukasjohnny2328@gmail.com"


/* The SMTP Session object used for Email sending */
SMTPSession smtp;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

// Go to the Project Settings (nut icon).
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "WLAN1-0762EH";
char pass[] = "Lukas@92";

const unsigned long eventInterval = 1000;
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;

unsigned long previousTime = 0;

const int buzzer = D3;
const unsigned long period = 1000;  //the value is a number of milliseconds

BlynkTimer timer;

int vibr_Pin_A =D0;
float returnPressure [3] = {0};
float returnVibration [2] = {0};

void bmpSensor()
{
  float Pressure = bmp.readPressure()/100; //refer to table hpa to pa
    
  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");
   
  Serial.print("Pressure = ");
  Serial.print(Pressure);
  Serial.println(" Pa");
   
  Serial.print("Approx altitude = ");
  Serial.print(bmp.readAltitude(1011.30)); // Current Pressure Hpa by sea Level Kuching
  Serial.println(" m");

  Serial.println();
  delay(2000);
  
  Blynk.virtualWrite(V0, bmp.readTemperature());
  Blynk.virtualWrite(V1, Pressure);
  Blynk.virtualWrite(V2, bmp.readAltitude(1011.30));

  returnPressure[0] = Pressure;
  returnPressure[1] = bmp.readTemperature();
  returnPressure[2] = bmp.readAltitude(1011.30);
  
}

void sendEmail(int _vibration, float _pressure, float _temperature, float _altitude)
{
  Serial.println();
  Serial.print("Connecting to AP");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  String _vib;
  _vib = String(_vibration);   //Converts integer to string

  String _pre;
  _pre = String(_pressure);   //Converts integer to string

   String _tem;
  _tem = String(_temperature);   //Converts integer to string

   String _alt;
  _alt = String(_altitude);   //Converts integer to string

  smtp.debug(1);

  /* Set the callback function to get the sending results */
  smtp.callback(smtpCallback);

  /* Declare the session config data */
  ESP_Mail_Session session;

  /* Set the session config */
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";
  /* Declare the message class */
  SMTP_Message message;

  /* Set the message headers */
  message.sender.name = "Warning Notifications";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "Natural Distater Alerting System";
  message.addRecipient("SAM", RECIPIENT_EMAIL);

  /*Send HTML message*/
  String textMsg = "Vibration Value : " + _vib + " m/s2" + "<br>" + "Pressure Value : " + _pre + " Pa" + "<br>" + "Temperature Value : " + _tem + " C" + "<br>" + "Altitude Value : " + _alt + " m";
  String htmlMsg = "<div style=\"color:#2f4468;\"><h2> Current Reading Value </h2><p>" + textMsg + "</p><p>- Sent from board System</p></div>";
  
  message.html.content = htmlMsg.c_str();
  message.html.content = htmlMsg.c_str();
  message.text.charSet = "us-ascii";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  //Send raw text message
//  String textMsg = "Vibration Value : " + _vib + " m/s2" + "\n" + "Pressure Value : " + _pre + " Pa" + "\n" + "Temperature Value : " + _tem + " C" + "\n" + "Altitude Value : " + _alt + " m";
//  message.text.content = textMsg.c_str();
//  message.text.charSet = "us-ascii";
//  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

  /* Set the custom message header */
  //message.addHeader("Message-ID: <abcde.fghij@gmail.com>");

  /* Connect to server with the session config */
  if (!smtp.connect(&session))
    return;

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email, " + smtp.errorReason());
}

void vibrationeSensor()
{
  long vibration =TP_init_A();    
  returnVibration[0] = vibration;  

  Blynk.virtualWrite(V3, vibration);
}

void checkCondition()
{
  if (returnVibration[0] > 1000 || returnPressure[0] > 5000 )
  {
    sendEmail(returnVibration[0],returnPressure[0],returnPressure[1],returnPressure[2]);       
    tone(buzzer, 10); // Send 1KHz sound signal...
    delay(1000);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(1000);        // ...for 1sec
  }
}

void setup() {
   Serial.begin(115200);
 
   pinMode(vibr_Pin_A, INPUT);

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
  
  timer.setInterval(500, bmpSensor);
  timer.setInterval(800, vibrationeSensor);
  timer.setInterval(1000, checkCondition);

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

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status){
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success()){
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;

    for (size_t i = 0; i < smtp.sendingResult.size(); i++){
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);

      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients);
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject);
    }
    Serial.println("----------------\n");
  }
}
