

const int LM1 = 2;  // Left motor input 1 (M1A)
const int LM2 = 3;  // Left motor input 2 (M1B)
const int RM1 = 4;  // Right motor input 1 (M2A)
const int RM2 = 5;  // Right motor input 2 (M2B)

void setup() {
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);

  Serial.begin(9600);
  
}

void loop() {
  int speed = 200; // PWM (0–255)

  //FORWARD
  analogWrite(RM1, speed);
  digitalWrite(RM2, LOW);
  analogWrite(LM1, speed);
  digitalWrite(LM2, LOW);
  delay(2000);

  //STOP
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, LOW);
  digitalWrite(LM1, LOW);
  digitalWrite(LM2, LOW);
  delay(1000);

  //REVERSE
  analogWrite(RM2, speed);
  digitalWrite(RM1, LOW);
  analogWrite(LM2, speed);
  digitalWrite(LM1, LOW);
  delay(2000);

  //STOP
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, LOW);
  digitalWrite(LM1, LOW);
  digitalWrite(LM2, LOW);
  delay(1000);
}

