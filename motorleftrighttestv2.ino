#include "CytronMotorDriver.h"
CytronMD motorLeft(PWM_PWM,3,5);
CytronMD motorRight(PWM_PWM,6,9);

void setup(){

}
void loop(){
  motorLeft.setSpeed(200);//moves forward
  motorRight.setSpeed(200);
  delay(1000);
  
  motorLeft.setSpeed(0);
  motorRight.setSpeed(0);
  delay(500);

  motorLeft.setSpeed(-200); //moves backward
  motorRight.setSpeed(-200);
  delay(2000);


  motorLeft.setSpeed(0);
  motorRight.setSpeed(0);
  delay(500);

  motorLeft.setSpeed(-200); //spin left
  motorRight.setSpeed(200);
  delay(2000);

  motorLeft.setSpeed(0); 
  motorRight.setSpeed(0);
  delay(1000);

  motorLeft.setSpeed(200); //spin right
  motorRight.setSpeed(-200);
  delay(2000);

  motorLeft.setSpeed(0); 
  motorRight.setSpeed(0);
  delay(500);
}
