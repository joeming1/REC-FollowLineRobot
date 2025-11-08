#include <Servo.h>
Servo myServo;  // create servo object to control a servo

void setup() {
  myServo.attach(4);  // attaches the servo on pin 4
}
void loop() {
  myServo.write(90);  // moves the servo to 90 degrees
  delay(1000);        // waits for a second
  myServo.write(0);   // moves the servo to 0 degrees
  delay(1000);        // waits for a second
}