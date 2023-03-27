/*
Here i'm using raspberry Pi Pico W to run this code.
Connect RX of Pixhawk to UARTO TX - GPO of Pico
Connect TX of Pixhawk to UARTO RX - GP1 of Pico
Refer to Raspberry Pi Pico W pin list for reference

//    Serial.print("Land L = ");
//    Serial.print(Array[4]);
//    Serial.print("  ");
//    Serial.print("Guided G = ");
//    Serial.print(Array[3]);
//    Serial.print("  ");
//    Serial.print("Throttle T = ");
//    Serial.print(Array[2]);
//    Serial.print("  ");
//    Serial.print("Axis X = ");
//    Serial.print(Array[0]);
//    Serial.print("  ");
//    Serial.print("Axis Y = ");
//    Serial.println(Array[1]);

*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(4, 5); // CE, CSN
const byte address[6] = "00001";
char receivedData[32] = "";
int xAxis, yAxis, Zaxis;

// Mavlink library
#include "C:\Users\Lukas\Documents\Arduino\libraries\mavlinkv2\common\mavlink.h"
unsigned long HeartbeatTime = 0;

int current_roll = 0;
int current_pitch = 0;
int current_throttle = 0;  //Min value is 1150 to run motors
int current_yaw = 0;

// Flag
int stabilize_flag = 0;
int arm_flag = 0;
int arm_on = 0;
int takeoff_flag = 0;
int land_flag = 0;

void setup() {
  Serial1.begin(57600);
  Serial.begin(57600);

  Serial.println("MAVLink starting.");

  radio.begin();
  radio.setAutoAck(false);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(0,address);
  radio.startListening();

}

unsigned long last_Time =0;

void receive_the_data()
{
  if(radio.available())
  {
    byte Array[6];
    radio.read(&Array, sizeof(Array));

    if (Array[4] == 1){
      land_flag +=1;
      if (land_flag < 2){
        Serial.println("Set Vehicle: Landed Mode");
        stabilize_flag = 0;
        arm_flag = 0;
        arm_on = 0;
        takeoff_flag = 0;
      }
    }

    else if (Array[3] == 1){
      stabilize_flag +=1;
      if (stabilize_flag < 2){
        Serial.println("Set Vehicle: Stabilize Mode");
        //Set to Stablize Mode
        CHANGEMODE(9);
        land_flag = 0;
        arm_flag = 1;
      }
    }
    
    int throttle = map(Array[2],0,255,1900,1045); //1050
    if ((throttle != 0) && (arm_flag==1)) {
      arm_on +=1;
      if (arm_on < 2) {
        Serial.println("Set Vehicle: Arm mode");
        ARM('a');
      }

      delay(5);
      
      if (takeoff_flag==1){
        Serial.println(throttle);
        current_roll = 0;
        current_pitch = 0;
        current_throttle = throttle;
        current_yaw = 0;
        OVERRIDE_RC(current_roll, current_pitch, current_throttle, current_yaw);
    }
    }
  }
}

void mavlink()
{
  // Mavlink Connections
  if ((millis() - HeartbeatTime) > 1000) {
  HeartbeatTime = millis();
  PIX_HEART_BEAT();
  }
}

void loop() {
  // Establish Mavink heartbeat
  mavlink();
  
  // NRF - receive data from Arduino Nano 
  receive_the_data();
 }

void PIX_HEART_BEAT() {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  uint16_t len;
  mavlink_msg_heartbeat_pack(1, 0, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_GENERIC, 0, 1, 0);  // System ID = 255 = GCS
  len = mavlink_msg_to_send_buffer(buf, &msg);

  Serial1.write(buf, len);
}

void ARM(char menu) {
  // 400 is referring to MAV_CMD_COMPONENT_ARM_DISARM (400 ). See https://mavlink.io/en/messages/common.html

  //Method 1
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];

  if (menu == 'a') {
    mavlink_msg_command_long_pack(255, 151, &msg, 1, 1, 400, 1, 1, 0, 0, 0, 0, 0, 0);
    
  } else if (menu = 'o') {
    mavlink_msg_command_long_pack(255, 151, &msg, 1, 1, 400, 1, 0, 0, 0, 0, 0, 0, 0);
  }

  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  Serial1.write(buf, len);
  takeoff_flag = 1;
}

void OVERRIDE_RC(int roll, int pitch, int throttle, int yaw) {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];

  mavlink_msg_rc_channels_override_pack(255, 151, &msg, 1, 0, roll, pitch, throttle, yaw, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);  // CHANNEL = 1(ROLL), 2(PITCH), 3(Throttle), 4(Yaw) , 5, 6, 7, 8

  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  Serial1.write(buf, len);
}

void CHANGEMODE(int mode) {
  //Set message variables
  uint8_t _system_id = 255;     // id of computer which is sending the command (ground control software has id of 255)
  uint8_t _component_id = 151;  // seems like it can be any # except the number of what Pixhawk sys_id is
  uint8_t _target_system = 1;   // Id # of Pixhawk (should be 1)
  uint8_t _base_mode = 1;
  uint32_t _custom_mode = mode;  //10 = auto mode

  /*
  Flight / Driving Modes (change custom mode above)
  0 - stabilize
  1 - acro
  2 - althold
  3 - auto
  4 - guided
  5 - loiter
  6 - RTL
  7 - Circle
  8 - 
  9 - Land
  10 - 
  11 - Drift
  12
  */

  // Initialize the required buffers
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];

  // Pack the message
  mavlink_msg_set_mode_pack(_system_id, _component_id, &msg, _target_system, _base_mode, _custom_mode);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);  // Send the message (.write sends as bytes)
//  Serial.println("\nsending set mode command...");
  Serial1.write(buf, len);  //Write data to serial port
}
