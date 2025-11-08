// --- PD Line Follower for Arduino Nano + TT Motors + 5-way TCRT5000 ---

// Sensor pins
const int sensorPins[5] = {A0, A1, A2, A3, A4};

// Motor pins
const int IN1 = 2;   // Left motor direction
const int IN2 = 3;   // Left motor speed (PWM)
const int IN3 = 4;   // Right motor direction
const int IN4 = 5;   // Right motor speed (PWM)

// PD constants (tune these)
float Kp = 25;     // proportional gain
float Kd = 15;     // derivative gain

// Motor base speed
int baseSpeed = 120;

// Variables
float error = 0;
float lastError = 0;

void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  int sensorValues[5];
  int weights[5] = {-2, -1, 0, 1, 2};  // leftmost to rightmost
  int total = 0, sum = 0;

  // Read sensors (analog)
  for (int i = 0; i < 5; i++) {
    sensorValues[i] = analogRead(sensorPins[i]);
    sensorValues[i] = map(sensorValues[i], 0, 1023, 0, 1);  // digital-like output
    sum += sensorValues[i] * weights[i];
    total += sensorValues[i];
  }

  // Calculate error
  if (total != 0) {
    error = (float)sum / total;
  } else {
    // line lost — optional fallback
    error = lastError > 0 ? 2 : -2;
  }

  // PD Control
  float P = error;
  float D = error - lastError;
  float PDvalue = (Kp * P) + (Kd * D);

  // Compute motor speeds
  int leftSpeed = baseSpeed + PDvalue;
  int rightSpeed = baseSpeed - PDvalue;

  // Limit speeds
    leftSpeed = constrain(leftSpeed, 0, 255);
    rightSpeed = constrain(rightSpeed, 0, 255);

  // Apply speeds
  analogWrite(IN2, leftSpeed);   // Left motor speed
  digitalWrite(IN1, HIGH);       // Left motor direction forward
  analogWrite(IN4, rightSpeed);  // Right motor speed
  digitalWrite(IN3, HIGH);       // Right motor direction forward

  // Save last error
  lastError = error;

  // Debug
  Serial.print("Error: "); Serial.print(error);
  Serial.print("\tPD: "); Serial.print(PDvalue);
  Serial.print("\tL: "); Serial.print(leftSpeed);
  Serial.print("\tR: "); Serial.println(rightSpeed);

  delay(10);
}
