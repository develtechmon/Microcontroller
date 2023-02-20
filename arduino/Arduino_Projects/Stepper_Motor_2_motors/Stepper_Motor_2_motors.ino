/*     Simple Stepper Motor Control Exaple Code
 *      
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
 */
// 10000000000000000000
// Defines pins numbers
const int stepPin = 3;
const int dirPin = 2;  

const int stepPin1 = 4;
const int dirPin1 = 5; 

int customDelay,customDelayMapped; // Defines variables
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT);
  pinMode(dirPin,OUTPUT);
   pinMode(stepPin1,OUTPUT);
  pinMode(dirPin1,OUTPUT);
 
  digitalWrite(dirPin,HIGH); //Enables the motor to move in a particular direction
  pinMode(dirPin1,OUTPUT);
}
void loop() {
  
  customDelayMapped = speedUp(); // Gets custom delay values from the custom speedUp function
  // Makes pules with custom delay, depending on the Potentiometer, from which the speed of the motor depends
  digitalWrite(stepPin, HIGH);
  digitalWrite(stepPin1, HIGH);
  delayMicroseconds(customDelayMapped);
  digitalWrite(stepPin, LOW);
  digitalWrite(stepPin1, LOW);
  delayMicroseconds(customDelayMapped);
}
// Function for reading the Potentiometer
int speedUp() {
  int customDelay = analogRead(A0); // Reads the potentiometer
  int newCustom = map(customDelay, 0, 1023, 300,4000); // Convrests the read values of the potentiometer from 0 to 1023 into desireded delay values (300 to 4000)
  return newCustom;  
}
