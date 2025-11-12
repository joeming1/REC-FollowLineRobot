#include "CytronMotorDriver.h"
#include <Servo.h>

Servo myServo;  

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
int stopspeed=0;
int box=0;
bool intersection=false;

unsigned long servostarttime=0;
bool servoactive=false;
int servoposition=0; 


void setup() {
  Serial.begin(9600);
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(sensorPins[i], INPUT);
  }
   myServo.attach(6);  
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
  //checkpoint
  if(!servoactive && box<3){
  if(sensorValues[0]> 500 && sensorValues[1]< 500 && sensorValues[3]< 500 && sensorValues[4]> 500){
      intersection=true;
    }}
  //drop box 
  if(intersection && !servoactive){ 
     myServo.write(90);  
     servostarttime=millis();        
     servoactive=true; 
     intersection=false;     
     box++;
  }
  //move while dropping
   if(servoactive){
    int slowspeed = 50; 
    motorLeft.setSpeed(slowspeed);
    motorRight.setSpeed(slowspeed);
    if(millis() - servostarttime >= 500){
        myServo.write(0);
        servoactive = false;
    }
    
}


  //move into endbox
  if(box==3 && sensorValues[0]< 500 && sensorValues[1]< 500 && sensorValues[3]< 500 && sensorValues[4]< 500){
    motorLeft.setSpeed(255);
    motorRight.setSpeed(255);
    delay(300);  
    motorLeft.setSpeed(0);
    motorRight.setSpeed(0);
    return;

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
  int leftSpeed =(int) (baseSpeed + correction);
  int rightSpeed =(int) (baseSpeed - correction);

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
