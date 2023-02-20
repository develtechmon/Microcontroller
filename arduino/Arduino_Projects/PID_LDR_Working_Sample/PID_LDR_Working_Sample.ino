#include <PID_v1.h>

double Setpoint, Input, Output;

#define PIN_INPUT 0
#define PIN_OUTPUT 3

//PID Parameters
double Kp=2, Ki=10, Kd=0;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void setup() {
  Serial.begin(9600);
  Input = analogRead(PIN_INPUT);
  Setpoint = 100;
  myPID.SetMode(AUTOMATIC);
}

void loop() {
  //Input = map(analogRead(PIN_INPUT), 0, 1024, 0, 255);
  Input = map(analogRead(PIN_INPUT), 0, 1024, 0, 255);
  myPID.Compute();
  analogWrite(PIN_OUTPUT, Output);
  Serial.print(Input);
  Serial.print(" ");
  Serial.println(Output);
  Serial.print(" ");
  Serial.println(Setpoint);

}
