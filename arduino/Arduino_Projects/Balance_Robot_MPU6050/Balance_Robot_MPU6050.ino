/*Arduino Self Balancing Robot
 * Code by: B.Aswinth Raj
 * Build on top of Lib: https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050
 * Website: circuitdigest.com 
 */
#include <AFMotor.h>

//defining motors
AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);

#include "I2Cdev.h"
#include <PID_v1.h> //From https://github.com/br3ttb/Arduino-PID-Library/blob/master/PID_v1.h
#include "MPU6050_6Axis_MotionApps20.h" //https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 mpu;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

/*********Tune these 4 values for your BOT*********/
//double setpoint= 176; //set the value when the bot is perpendicular to ground using serial monitor. 
double setpoint= 190; //set the value when the bot is perpendicular to ground using serial monitor. 

//Read the project documentation on circuitdigest.com to learn how to set these values
double Kp = 20; //Set this first
double Kd = 1; //Set this secound
double Ki = 140; //Finally set this 
/******End of values setting*********/

/*****Interrupt Pin*************/
#define INTERRUPT_PIN 18

double input, output;
PID pid(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady()
{
    mpuInterrupt = true;
}

void setup() {
     // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

  Serial.begin(115200);

  // initialize device
    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);


     // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // load and configure the DMP
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
   // mpu.setXGyroOffset(220);
   // mpu.setYGyroOffset(76);
   // mpu.setZGyroOffset(-85);
   // mpu.setZAccelOffset(1688); 

    mpu.setXGyroOffset(-255);
    mpu.setYGyroOffset(-340);
    mpu.setZGyroOffset(-288);
    mpu.setZAccelOffset(11395);

      // make sure it worked (returns 0 if so)
    if (devStatus == 0)
    {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
        Serial.println(F(")..."));
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        //attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
        
        //setup PID
        pid.SetMode(AUTOMATIC);
        pid.SetSampleTime(10);
        pid.SetOutputLimits(-255, 255);  
    }
    else
    {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }


//By default turn off both the motors
    //analogWrite(6,LOW);
    //analogWrite(9,LOW);
    //analogWrite(10,LOW);
   // analogWrite(11,LOW);
}

void loop() {
 
    // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize)
    {
        //no mpu data - performing PID calculations and output to motors     
        pid.Compute();   
        
        //Print the value of Input and Output on serial monitor to check how it is working.
        Serial.print(input); Serial.print(" =>"); Serial.println(output);
               
        if (input>150 && input<200){//If the Bot is falling 
          
        if (output>0) //Falling towards front 
        Forward(); //Rotate the wheels forward 
        else if (output<0) //Falling towards back
        Reverse(); //Rotate the wheels backward 
        }
        else //If Bot not falling
        Stop(); //Hold the wheels still
        
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024)
    {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    }
    else if (mpuIntStatus & 0x02)
    {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        mpu.dmpGetQuaternion(&q, fifoBuffer); //get value for q
        mpu.dmpGetGravity(&gravity, &q); //get value for gravity
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity); //get value for ypr

        input = ypr[1] * 180/M_PI + 180;

   }
}

void Forward() //Code to rotate the wheel forward 
{
   //analogWrite(6,output);
    //analogWrite(9,0);
   // analogWrite(10,output);
   // analogWrite(11,0);

    motor1.run(FORWARD);
    motor1.setSpeed(output);

    motor3.run(FORWARD);
    motor3.setSpeed(output);
    Serial.print("F"); //Debugging information 
}

void Reverse() //Code to rotate the wheel Backward  
{
    motor1.run(BACKWARD);
    motor1.setSpeed(output*-1);

    motor3.run(BACKWARD);
    motor3.setSpeed(output*-1);
    Serial.print("R");
}

void Stop() //Code to stop both the wheels
{
    motor1.run(RELEASE);
    motor1.setSpeed(0);

    motor3.run(RELEASE);
    motor3.setSpeed(0); 
    Serial.print("S");
}
