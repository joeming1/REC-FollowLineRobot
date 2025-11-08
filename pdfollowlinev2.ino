#include "CytronMotorDriver.h"

//Motor setup
CytronMD motorLeft(PWM_PWM, 2, 3);   //Left motor on M1
CytronMD motorRight(PWM_PWM, 4, 5);  //Right motor on M2

//Sensor setup 
#define NUM_SENSORS 5
int sensorPins[NUM_SENSORS] = {A0, A1, A2, A3, A4};
int sensorValues[NUM_SENSORS];
int sensorWeight[NUM_SENSORS] = {-2, -1, 0, 1, 2}; //Leftmost = -2 rightmost = +2

//PD control variables
double Kp = 1;     
double Kd = 1;     
double lastError = 0;
double error = 0;
double correction = 0;

//Motor speed settings 
int baseSpeed = 150;  //Base motor speed 
int maxSpeed = 255;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(sensorPins[i], INPUT);
  }
}

void loop() {
  //Read sensors
  int total = 0;
  int count = 0;

  for (int i = 0; i < NUM_SENSORS; i++) {
    sensorValues[i] = analogRead(sensorPins[i]);
    if (sensorValues[i] < 500) { //Detect black line (adjust threshold)
      total += sensorWeight[i];
      count++;
    }
  }

  //Calculate error 
  if (count != 0) {
    error = (double)total / count;
  } else {
    //No line detected, use last error to guess direction
    if (lastError < 0)
      error = -2;
    else
      error = 2;
  }

  //PD calculation
  float P = error;
  float D = error - lastError;
  correction = (Kp * P) + (Kd * D);
  lastError = error;

  //Calculate motor speeds 
  int leftSpeed = baseSpeed + correction;
  int rightSpeed = baseSpeed - correction;

  //Constrain speeds
  leftSpeed = constrain(leftSpeed, -maxSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, -maxSpeed, maxSpeed);

  //Set motor speeds
  motorLeft.setSpeed(leftSpeed);
  motorRight.setSpeed(rightSpeed);

  //debugging
  Serial.print("Error: "); Serial.print(error);
  Serial.print("  Correction: "); Serial.print(correction);
  Serial.print("  Left: "); Serial.print(leftSpeed);
  Serial.print("  Right: "); Serial.println(rightSpeed);

  delay(10); //small delay for stability
}
