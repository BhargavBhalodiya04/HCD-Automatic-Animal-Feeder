#include <SoftwareSerial.h>

// Ultrasonic Sensor Pins
#define TRIG_PIN 12
#define ECHO_PIN 11

// LED Pins
#define RED_LED A0
#define BLUE_LED A1
#define GREEN_LED A2

// Motor A Pins
const int ENA = 5;
const int IN1 = 6;
const int IN2 = 7;

// Motor B Pins
const int IN3 = 8;
const int IN4 = 9;
const int ENB = 10;

// Bluetooth on pins 2 (RX), 3 (TX)
SoftwareSerial bluetooth(2, 3); // RX, TX

// Timing and motor control
unsigned long delayStartTime = 0;
unsigned long motorStartTime = 0;
unsigned long delayDuration = 0;
unsigned long motorRunDuration = 60000UL; // Default 1 minute
unsigned long speedReduceDelay = 10;      // 10 ms delay before reducing speed
bool waitingToStart = false;
bool motorRunning = false;
bool speedReduced = false;

void setup() {
  // Ultrasonic Sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // LEDs
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  // Motors
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Serial and Bluetooth
  Serial.begin(9600);
  bluetooth.begin(9600);

  Serial.println("System Ready. Distance Sensor + LED + Motor Control + Bluetooth Echo");
  bluetooth.println("System Ready. Send 1-9 for delay, followed by 1-9 for rotation time.");
}

long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2;  // Convert to cm
  return distance;
}

void loop() {
  // === Bluetooth Echo + Motor Commands ===
  if (bluetooth.available()) {
    char input = bluetooth.read();
    Serial.print("BT: ");
    Serial.println(input);
    bluetooth.print("You sent: ");
    bluetooth.println(input);

    if (input == '0') {
      stopMotors();
      waitingToStart = false;
      bluetooth.println("Motors stopped by user.");
      Serial.println("Motors stopped by user.");
    }
    else if (input >= '1' && input <= '9') {
      // First number for delay before motor start
      int delaySec = input - '0';
      delayDuration = delaySec * 1000UL;
      delayStartTime = millis();
      waitingToStart = true;
      motorRunning = false;
      speedReduced = false;

      bluetooth.print("Motors will start after ");
      bluetooth.print(delaySec);
      bluetooth.println(" seconds.");
      Serial.print("Motors will start after ");
      Serial.print(delaySec);
      Serial.println(" seconds.");
    }
    else if (input >= 'a' && input <= 'i') {
      // Second number for rotation time (1-9, each representing 10-90 seconds)
      int rotationSec = (input - 'a' + 1) * 10;
      motorRunDuration = rotationSec * 1000UL; // Convert to milliseconds
      bluetooth.print("Motor rotation time set to ");
      bluetooth.print(rotationSec);
      bluetooth.println(" seconds.");
      Serial.print("Motor rotation time set to ");
      Serial.print(rotationSec);
      Serial.println(" seconds.");
    }
  }

  // Also allow sending from Serial Monitor
  if (Serial.available()) {
    char c = Serial.read();
    bluetooth.write(c);
  }

  // === Motor Start After Delay ===
  if (waitingToStart && (millis() - delayStartTime >= delayDuration)) {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, 100); // Motor A
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, 100); // Motor B

    motorStartTime = millis();
    motorRunning = true;
    waitingToStart = false;

    bluetooth.println("Motors started at high speed.");
    Serial.println("Motors started at high speed.");
  }

  // === Reduce Motor Speed After Short Time ===
  if (motorRunning && !speedReduced && millis() - motorStartTime >= speedReduceDelay) {
    analogWrite(ENA, 40);
    analogWrite(ENB, 40);
    speedReduced = true;

    bluetooth.println("Motor speeds reduced to 40.");
    Serial.println("Motor speeds reduced to 40.");
  }

  // === Stop Motors After Custom Rotation Time ===
  if (motorRunning && millis() - motorStartTime >= motorRunDuration) {
    stopMotors();
    bluetooth.println("Motors stopped after custom rotation time.");
    Serial.println("Motors stopped after custom rotation time.");
  }

  // === Distance Sensing + LED Feedback ===
  long distance = getDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance <= 10) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
  } else if (distance <= 30) {
    digitalWrite(RED_LED, LOW);
    digitalWrite(BLUE_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
  } else if (distance <= 100) {
    digitalWrite(RED_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
  } else {
    digitalWrite(RED_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
  }

  delay(500); // Delay for sensor and LED stability
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  motorRunning = false;
  speedReduced = false;
}
