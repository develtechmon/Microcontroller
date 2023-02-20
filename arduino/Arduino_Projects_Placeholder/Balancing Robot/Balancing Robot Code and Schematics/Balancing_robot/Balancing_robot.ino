#include <SoftwareSerial.h>   //Include SoftwareSerial for bluetooth connection

#include <AFMotor.h>    //You will need to download and install the adafruit motor shield library. The information you need will be on adafruit's website

#include <Wire.h>   //Include Wire for connection with the mpu6050

AF_DCMotor motor1(1,MOTOR12_8KHZ);    //Initialize motor variables for the two motors connected to the shield 
AF_DCMotor motor2(2,MOTOR12_8KHZ);

int bluetoothTx = 0;    //Initialize variables for the transmit and recieve pins for the bluetooth connection 
int bluetoothRx = 1;

int bluetoothval;   //This is the variable we store the bluetooth data in

int gyro_address = 0x68;    //This is the mpu6050's address needed to access the module                                   
int acc_calibration_value = -6928;    //This is the balancing robot's balance point, use the other program to find it.                        

//Various settings
float pid_p_gain = 12.8;    //Pid gain values  THESE CHANGE FOR EACH ROBOT!                           
float pid_i_gain = .028;                                     
float pid_d_gain = 65;  
                                   
float turning_speed = 30;   //These are constants determining the driving and turing speed                                 
float max_target_speed = 100;                                


byte start;   //Tells the robot to start balancing

int receive_counter;    //This is used for calibrating the gyro

int gyro_pitch_data_raw, gyro_yaw_data_raw, accelerometer_data_raw;   //Gyro data variables

long gyro_yaw_calibration_value, gyro_pitch_calibration_value;    //Gyro calibration data variables

unsigned long loop_timer;   //Times the loop

float angle_gyro, angle_acc, angle, self_balance_pid_setpoint;    //More Gyro data variables
float pid_error_temp, pid_i_mem, pid_setpoint, gyro_input, pid_output, pid_last_d_error;    //Pid variables
float pid_output_left, pid_output_right;    //Pid output for each motor

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);   //Create the bluetooth variable/Object

void setup(){
  
  bluetooth.begin(115200);
  bluetooth.print("$");  
  bluetooth.print("$");
  bluetooth.print("$"); 
  delay(100);  
  bluetooth.println("U,9600,N");
  bluetooth.begin(9600);
   Serial.begin(38400);
  //Serial.begin(9600);                                                       
  Wire.begin();                                                           
  TWBR = 12;                                                        
  OCR2A = 39;
  
  Wire.beginTransmission(gyro_address);                                     //Start communication with the address found during search.
  Wire.write(0x6B);                                                         //We want to write to the PWR_MGMT_1 register (6B hex)
  Wire.write(0x00);                                                         //Set the register bits as 00000000 to activate the gyro
  Wire.endTransmission();                                                   //End the transmission with the gyro.
  //Set the full scale of the gyro to +/- 250 degrees per second
  Wire.beginTransmission(gyro_address);                                     //Start communication with the address found during search.
  Wire.write(0x1B);                                                         //We want to write to the GYRO_CONFIG register (1B hex)
  Wire.write(0x00);                                                         //Set the register bits as 00000000 (250dps full scale)
  Wire.endTransmission();                                                   //End the transmission with the gyro
  //Set the full scale of the accelerometer to +/- 4g.
  Wire.beginTransmission(gyro_address);                                     //Start communication with the address found during search.
  Wire.write(0x1C);                                                         //We want to write to the ACCEL_CONFIG register (1A hex)
  Wire.write(0x08);                                                         //Set the register bits as 00001000 (+/- 4g full scale range)
  Wire.endTransmission();                                                   //End the transmission with the gyro
  //Set some filtering to improve the raw data.
  Wire.beginTransmission(gyro_address);                                     //Start communication with the address found during search
  Wire.write(0x1A);                                                         //We want to write to the CONFIG register (1A hex)
  Wire.write(0x03);                                                         //Set the register bits as 00000011 (Set Digital Low Pass Filter to ~43Hz)
  Wire.endTransmission();                                                   //End the transmission with the gyro 

  pinMode(13, OUTPUT);                                                      //Set onboard led to output
 
  
  
    motor1.setSpeed(0);                                                     //Make sure the motors dont turn until calibration is complete
    motor2.setSpeed(0);    

  for(receive_counter = 0; receive_counter < 500; receive_counter++){       //Create 500 loops
    if(receive_counter % 15 == 0)digitalWrite(13, !digitalRead(13));        //Change the state of the LED every 15 loops to make the LED blink fast
    Wire.beginTransmission(gyro_address);                                   //Start communication with the gyro
    Wire.write(0x43);                                                       //Start reading the Who_am_I register 75h
    Wire.endTransmission();                                                 //End the transmission
    Wire.requestFrom(gyro_address, 4);                                      //Request 2 bytes from the gyro
    gyro_yaw_calibration_value += Wire.read()<<8|Wire.read();               //Combine the two bytes to make one integer
    gyro_pitch_calibration_value += Wire.read()<<8|Wire.read();             //Combine the two bytes to make one integer
    delayMicroseconds(3700);                                                //Wait for 3700 microseconds to simulate the main program loop time
  }
    gyro_pitch_calibration_value /= 500;                                    //Divide the total value by 500 to get the avarage gyro offset
    gyro_yaw_calibration_value /= 500;                                      //Divide the total value by 500 to get the avarage gyro offset

    loop_timer = micros() + 4000;                                           //Set the loop_timer variable at the next end loop time    
     
    Serial.println("GO");                                                   //Just for debugging to see if it is working, you can take this out          
                                                      
  }

void loop(){

  if(bluetooth.available()){                                               //Checks to see if any driving value was sent
    bluetoothval = bluetooth.read();                                       //Sets bluetoothval to the incoming integer
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //Angle calculations
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Wire.beginTransmission(gyro_address);                                     //Start communication with the gyro
  Wire.write(0x3F);                                                         //Start reading at register 3F
  Wire.endTransmission();                                                   //End the transmission
  Wire.requestFrom(gyro_address, 2);                                        //Request 2 bytes from the gyro
  accelerometer_data_raw = Wire.read()<<8|Wire.read();                      //Combine the two bytes to make one integer
  accelerometer_data_raw += acc_calibration_value;                          //Add the accelerometer calibration value
  if(accelerometer_data_raw > 8200)accelerometer_data_raw = 8200;           //Prevent division by zero by limiting the acc data to +/-8200;
  if(accelerometer_data_raw < -8200)accelerometer_data_raw = -8200;         //Prevent division by zero by limiting the acc data to +/-8200;

  angle_acc = asin((float)accelerometer_data_raw/8200.0)* 57.296;           //Calculate the current angle according to the accelerometer

  if(start == 0 && angle_acc > -0.5&& angle_acc < 0.5){                     //If the accelerometer angle is almost 0
    angle_gyro = angle_acc;                                                 //Load the accelerometer angle in the angle_gyro variable
    start = 1;                                                              //Set the start variable to start the PID controller
  }
  
  Wire.beginTransmission(gyro_address);                                     //Start communication with the gyro
  Wire.write(0x43);                                                         //Start reading at register 43
  Wire.endTransmission();                                                   //End the transmission
  Wire.requestFrom(gyro_address, 4);                                        //Request 4 bytes from the gyro
  gyro_yaw_data_raw = Wire.read()<<8|Wire.read();                           //Combine the two bytes to make one integer
  gyro_pitch_data_raw = Wire.read()<<8|Wire.read();                         //Combine the two bytes to make one integer
  
  gyro_pitch_data_raw -= gyro_pitch_calibration_value;                      //Add the gyro calibration value
  angle_gyro += gyro_pitch_data_raw * 0.000031;                             //Calculate the traveled during this loop angle and add this to the angle_gyro variable
  
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //MPU-6050 offset compensation
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //Not every gyro is mounted 100% level with the axis of the robot. This can be cause by misalignments during manufacturing of the breakout board. 
  //As a result the robot will not rotate at the exact same spot and start to make larger and larger circles.
  //To compensate for this behavior a VERY SMALL angle compensation is needed when the robot is rotating.
  //Try 0.0000003 or -0.0000003 first to see if there is any improvement.

  gyro_yaw_data_raw -= gyro_yaw_calibration_value;                          //Add the gyro calibration value
  //Uncomment the following line to make the compensation active
  angle_gyro -= gyro_yaw_data_raw * 0.0000003;                            //Compensate the gyro offset when the robot is rotating

  angle_gyro = angle_gyro * 0.9996 + angle_acc * 0.0004;                    //Correct the drift of the gyro angle with the accelerometer angle

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //PID controller calculations
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //The balancing robot is angle driven. First the difference between the desired angel (setpoint) and actual angle (process value)
  //is calculated. The self_balance_pid_setpoint variable is automatically changed to make sure that the robot stays balanced all the time.
  //The (pid_setpoint - pid_output * 0.015) part functions as a brake function.
  pid_error_temp = angle_gyro - self_balance_pid_setpoint - pid_setpoint;
  if(pid_output > 10 || pid_output < -10)pid_error_temp += pid_output * 0.015 ;

  pid_i_mem += pid_i_gain * pid_error_temp;                                 //Calculate the I-controller value and add it to the pid_i_mem variable
 if(pid_i_mem > 400)pid_i_mem = 400;                                       //Limit the I-controller to the maximum controller output
 else if(pid_i_mem < -400)pid_i_mem = -400;
  //Calculate the PID output value
  pid_output = pid_p_gain * pid_error_temp + pid_i_mem + pid_d_gain * (pid_error_temp - pid_last_d_error);
  if(pid_output > 400)pid_output = 400;                                     //Limit the PI-controller to the maximum controller output
  else if(pid_output < -400)pid_output = -400;

  pid_last_d_error = pid_error_temp;                                        //Store the error for the next loop

  if(pid_output < 5 && pid_output > -5)pid_output = 0;                      //Create a dead-band to stop the motors when the robot is balanced

  if(angle_gyro > 30 || angle_gyro < -30 || start == 0 || low_bat == 1){    //If the robot tips over or the start variable is zero or the battery is empty
    pid_output = 0;                                                         //Set the PID controller output to 0 so the motors stop moving
    pid_i_mem = 0;                                                          //Reset the I-controller memory
    start = 0;                                                              //Set the start variable to 0
    self_balance_pid_setpoint = 0;                                          //Reset the self_balance_pid_setpoint variable
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //Control calculations
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
     pid_output_left = pid_output;                                             //Copy the controller output to the pid_output_left variable for the left motor
  pid_output_right = pid_output; 
                                                                            //Copy the controller output to the pid_output_right variable for the right motor

  if(bluetoothval == 1){                                           
    pid_output_left += turning_speed + 20;                                       //Increase the left motor speed
    pid_output_right -= turning_speed;                                      //Decrease the right motor speed
  }
  if(bluetoothval == 2){                                            
    pid_output_left -= turning_speed + 20;                                       //Decrease the left motor speed
    pid_output_right += turning_speed;                                      //Increase the right motor speed
  }

  if(bluetoothval == 3){                                            
    if(pid_setpoint > -2.5)pid_setpoint -= 0.05;                            //Slowly change the setpoint angle so the robot starts leaning forewards
    if(pid_output > max_target_speed * -1)pid_setpoint -= 0.005;
    pid_output_left += 30;//Slowly change the setpoint angle so the robot starts leaning forewards
  }

  
  if(bluetoothval == 4){                                            
    if(pid_setpoint < 2.5)pid_setpoint += 0.05;                             //Slowly change the setpoint angle so the robot starts leaning backwards
    if(pid_output < max_target_speed)pid_setpoint += 0.005;                 //Slowly change the setpoint angle so the robot starts leaning backwards
  } 
  

  if(bluetoothval == 0){                                         //Slowly reduce the setpoint to zero if no foreward or backward command is given
    if(pid_setpoint > 0.5)pid_setpoint -=0.05;                              //If the PID setpoint is larger then 0.5 reduce the setpoint with 0.05 every loop
    else if(pid_setpoint < -0.5)pid_setpoint +=0.05;                        //If the PID setpoint is smaller then -0.5 increase the setpoint with 0.05 every loop
    else pid_setpoint = 0;                                                  //If the PID setpoint is smaller then 0.5 or larger then -0.5 set the setpoint to 0
  }

  
  
  //The self balancing point is adjusted when there is not forward or backwards movement from the transmitter. This way the robot will always find it's balancing point
  if(pid_setpoint == 0){                                                    //If the setpoint is zero degrees
    if(pid_output < 0)self_balance_pid_setpoint += 0.0015;                  //Increase the self_balance_pid_setpoint if the robot is still moving forewards
    if(pid_output > 0)self_balance_pid_setpoint -= 0.0015;                  //Decrease the self_balance_pid_setpoint if the robot is still moving backwards
  }

  


    motor2.setSpeed(abs(pid_output_left));    //Sets the motor speed to the pid value
    motor1.setSpeed(abs(pid_output_right));
    
    if(pid_output_left < 0){                  //If the pid value is negative the motor need to go backwards
       motor2.run(FORWARD);
   
    }
    else if(pid_output_left > 0){             //If the pid value is positive the motor needs to go forward
       motor2.run(BACKWARD);
       
    }
    else if(pid_output_left == 0){            //If the pid value is 0 the motor can stop moving
      motor2.run(RELEASE);
  
    }
     if(pid_output_right < 0){
     
        motor1.run(FORWARD);
    }
    else if(pid_output_right > 0){
     
        motor1.run(BACKWARD);
    }
    else if(pid_output_right == 0){
    
      motor1.run(RELEASE);
    }
}


























