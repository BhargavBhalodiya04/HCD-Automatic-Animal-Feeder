// Motor 1 Pins
int ena = 5;  // Speed Control (PWM)
int in1 = 6;  // Direction Control
int in2 = 7;  // Direction Control

// Motor 2 Pins
int enb = 10; // Speed Control (PWM)
int in3 = 8;  // Direction Control
int in4 = 9;  // Direction Control

void setup() {
  // Set motor pins as OUTPUT
  pinMode(ena, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  
  pinMode(enb, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop() {
  // Wait 5 seconds before starting motors
  // delay(5000);

  // Start Motor 1 (Clockwise)
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(ena, 100); // Full speed (0-255)

  // Start Motor 2 (Clockwise)
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enb, 100); // Full speed (0-255)

  // Run motors for a short duration (adjust as needed for one rotation)
  delay(5000);  // Motors run for 1 second

  // Stop Motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(ena, 0);

  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enb, 0);

  // Wait 5 seconds before running again
  delay(2000);
}
