#include <Servo.h>
Servo myServo;
// Motor pins for L298N
#define ENA 11   // PWM for Left Motor
#define IN1 8
#define IN2 9

#define ENB 5     // PWM for Right Motor
#define IN3 6
#define IN4 7


// Sensors
#define NUM_SENSORS 4
int sensorPins[NUM_SENSORS] = {A3, A4, A6, A7};
int sensorValues[NUM_SENSORS];

// Motor speeds
int baseSpeed = 90;  // Forward speed
int slightTurnSpeed = 30; // For gentle turns
int spinSpeed = 120;  // For sharp turns
int interval = 0;

// Interval-based checkpoint variables
bool seenBlack[NUM_SENSORS] = {false, false, false, false};
unsigned long intervalStart = 0;


void setup() {
  Serial.begin(9600);

  // Motor pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Sensor pins
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(sensorPins[i], INPUT);
  }
  myServo.attach(3);
  myServo.write(170);
  setMotorSpeed(baseSpeed, baseSpeed);
  delay(400);

 
}
 unsigned long currTime = millis();
void loop() {
  // Read sensors
  for (int i = 0; i < NUM_SENSORS; i++) {
    sensorValues[i] = analogRead(sensorPins[i]);
  }

  bool sharpLeft = sensorValues[0] < 500;   // Outer left
  bool left = sensorValues[1] < 500;        // Inner left
  bool right = sensorValues[2] < 500;       // Inner right
  bool sharpRight = sensorValues[3] < 500 ;  // Outer right

  bool center = sensorValues[0] > 500 && sensorValues[1] > 500 &&
                sensorValues[2] > 500 && sensorValues[3] > 500;

if (center) {
    setMotorSpeed(baseSpeed, baseSpeed); // Go straight
}
else if (sharpLeft) {
  // Reset for next interval
    for (int i = 0; i < NUM_SENSORS; i++) seenBlack[i] = false;
    // Instead of delay(400), use a small loop
    unsigned long startTime = millis();
    while (millis() - startTime < 500) {
        // Keep moving forward during this pre-spin delay
        interval = millis() - startTime;
        setMotorSpeed(baseSpeed, baseSpeed);
        // Read sensors during this interval
        for (int i = 0; i < NUM_SENSORS; i++) {
            sensorValues[i] = analogRead(sensorPins[i]);
            // Mark sensor as having seen black for checkpoint
            if (sensorValues[i] < 500) {
                seenBlack[i] = true;
            }
        }

        // If all sensors saw black at least once in the interval → checkpoint
        bool allSeen = true;
        for (int i = 0; i < NUM_SENSORS; i++) {
            if (!seenBlack[i]) {
                allSeen = false;
                break;
            }
        }


        if (allSeen && ((startTime - currTime) > 4000)) {

          Serial.println("Dropping cube");
          setMotorSpeed(baseSpeed, baseSpeed); 
          delay(300);           
            // Trigger checkpoint
            stopMotors();
            myServo.write(120);
            delay(1000);
            myServo.write(170);
            delay(1000);
            setMotorSpeed(spinSpeed, 0);
           delay(interval * 3);
           
            intervalStart = millis();
            return; // Exit loop and go to next iteration of loop()
        }

      
    }

    // Now do the spin routine
    sensorValues[1] = analogRead(sensorPins[1]);
    while(sensorValues[1] > 500){
        sensorValues[1] = analogRead(sensorPins[1]);
        setMotorSpeed(spinSpeed, -spinSpeed); // Spin left
    }
       setMotorSpeed(spinSpeed, -spinSpeed); 
     delay(30);
    

    stopMotors();
}
 
else if (sharpRight) {

    for (int i = 0; i < NUM_SENSORS; i++) seenBlack[i] = false;
    unsigned long startTime = millis();
    while (millis() - startTime < 500) {
        interval = millis() - startTime;
        setMotorSpeed(baseSpeed, baseSpeed);
        for (int i = 0; i < NUM_SENSORS; i++) {
            sensorValues[i] = analogRead(sensorPins[i]);
            if (sensorValues[i] < 500) {
                seenBlack[i] = true;
            }
        }

        bool allSeen = true;
        for (int i = 0; i < NUM_SENSORS; i++) {
            if (!seenBlack[i]) {
                allSeen = false;
                break;
            }
        }

        if (allSeen && ((startTime - currTime) > 4000)) {  
          Serial.println("Dropping cube");
          setMotorSpeed(baseSpeed, baseSpeed); 
          delay(300);
            stopMotors();
            myServo.write(120);
            delay(1000);
            myServo.write(170);
            delay(1000);
          setMotorSpeed(0, spinSpeed);
          delay(interval * 3);
            

            intervalStart = millis();
            return;
        }

     
    }

    sensorValues[2] = analogRead(sensorPins[2]);
    while(sensorValues[2] > 500){
        sensorValues[2] = analogRead(sensorPins[2]);
        setMotorSpeed(-spinSpeed, spinSpeed); // Spin right
    }
     setMotorSpeed(-spinSpeed, spinSpeed); 
     delay(30);
  
  
    
    stopMotors();
}

else if (left) {
    setMotorSpeed(baseSpeed, slightTurnSpeed); // Slight left
    delay(30);
}
else if (right) {
    setMotorSpeed(slightTurnSpeed, baseSpeed); // Slight right
    delay(30);
}
else {
  setMotorSpeed(baseSpeed,baseSpeed);
  delay(1000);}


  delay(20); // Small delay for stability
}

// Motor control functions
void setMotorSpeed(int leftSpeed, int rightSpeed) {
  // Left Motor
  if (leftSpeed > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else if (leftSpeed < 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    leftSpeed = -leftSpeed;
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
  analogWrite(ENA, constrain(leftSpeed, 0, 255));

  // Right Motor
  if (rightSpeed > 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else if (rightSpeed < 0) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    rightSpeed = -rightSpeed;
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
  analogWrite(ENB, constrain(rightSpeed, 0, 255));
}

void stopMotors() {
  setMotorSpeed(0, 0);
}