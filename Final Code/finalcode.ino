
#include <SoftwareSerial.h>

// Ultrasonic Sensor Pins
#define TRIG_PIN 12
#define ECHO_PIN 11

// LED Pins
#define RED_LED A0
#define BLUE_LED A1
#define GREEN_LED A2

// Motor A Pins
const int motorA1 = 8;
const int motorA2 = 9;
const int enableA = 10;

// Motor B Pins
const int motorB1 = 7;
const int motorB2 = 6;
const int enableB = 5;

// Bluetooth Module
SoftwareSerial bluetooth(2, 3); // RX, TX

// State Variables
float delayTimeInMs = 1000;  // Default 1s
int rotationTimeInSeconds = 1;
bool startCycle = false;
bool isPaused = false;
bool cancelCycle = false;
bool motorRunning = false;
unsigned long lastActionTime = 0;
String inputData = "";

// Distance and LED
long getDistance() {
  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

// Setup
void setup() {
  // Ultrasonic Sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // LEDs
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  // Motors
  pinMode(motorA1, OUTPUT); pinMode(motorA2, OUTPUT); pinMode(enableA, OUTPUT);
  pinMode(motorB1, OUTPUT); pinMode(motorB2, OUTPUT); pinMode(enableB, OUTPUT);

  // Start Serial Communication
  Serial.begin(9600);
  bluetooth.begin(9600);

  Serial.println("System Ready.");
  bluetooth.println("System Ready. Use 'delay,rotation' or commands 'p'=pause, 'c'=cancel.");
}

// Stop motor function
void stopMotors() {
  digitalWrite(enableA, LOW);
  digitalWrite(enableB, LOW);
  digitalWrite(motorA1, LOW); digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW); digitalWrite(motorB2, LOW);
  motorRunning = false;
}

// Start motor function
void startMotors() {
  digitalWrite(motorA1, HIGH); digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, HIGH); digitalWrite(motorB2, LOW);
  digitalWrite(enableA, HIGH); digitalWrite(enableB, HIGH);
  motorRunning = true;
}

void handleLEDs(long distance) {
  if (distance <= 10) {
    digitalWrite(RED_LED, HIGH); digitalWrite(BLUE_LED, LOW); digitalWrite(GREEN_LED, LOW);
  } else if (distance <= 30) {
    digitalWrite(RED_LED, LOW); digitalWrite(BLUE_LED, HIGH); digitalWrite(GREEN_LED, LOW);
  } else if (distance <= 100) {
    digitalWrite(RED_LED, LOW); digitalWrite(BLUE_LED, LOW); digitalWrite(GREEN_LED, HIGH);
  } else {
    digitalWrite(RED_LED, LOW); digitalWrite(BLUE_LED, LOW); digitalWrite(GREEN_LED, LOW);
  }
}

void loop() {
  // Handle Bluetooth Input
  while (bluetooth.available()) {
    char received = bluetooth.read();
    if (received == '\n' || received == '\r') {
      inputData.trim();

      if (inputData.length() == 0) continue;

      if (inputData.equalsIgnoreCase("p")) {
        isPaused = !isPaused;
        String msg = isPaused ? "⏸ Paused." : "▶ Resumed.";
        Serial.println(msg); bluetooth.println(msg);
      } else if (inputData.equalsIgnoreCase("c")) {
        cancelCycle = true;
        stopMotors();
        startCycle = false;
        Serial.println("❌ Canceled."); bluetooth.println("❌ Canceled.");
      } else if (inputData.indexOf(',') > 0) {
        int idx = inputData.indexOf(',');
        float delayHrs = inputData.substring(0, idx).toFloat();
        int rotSec = inputData.substring(idx + 1).toInt();

        if (delayHrs > 0 && rotSec > 0) {
          delayTimeInMs = delayHrs * 3600 * 1000UL;
          rotationTimeInSeconds = rotSec;
          startCycle = true;
          cancelCycle = false;
          motorRunning = false;
          lastActionTime = millis();
          String msg = "✅ Delay: " + String(delayHrs) + "h, Rotation: " + String(rotSec) + "s";
          Serial.println(msg); bluetooth.println(msg);
        } else {
          bluetooth.println("❌ Invalid format or values.");
        }
      } else {
        bluetooth.println("❌ Invalid command.");
      }

      inputData = "";
    } else {
      inputData += received;
    }
  }

  // Motor timing logic
  unsigned long currentMillis = millis();

  if (startCycle && !cancelCycle) {
    if (!motorRunning && currentMillis - lastActionTime >= delayTimeInMs && !isPaused) {
      startMotors();
      lastActionTime = currentMillis;
      Serial.println("🟢 Motors ON"); bluetooth.println("🟢 Motors ON");
    }

    if (motorRunning && currentMillis - lastActionTime >= rotationTimeInSeconds * 1000UL && !isPaused) {
      stopMotors();
      Serial.println("🔴 Motors OFF"); bluetooth.println("🔴 Motors OFF");
      startCycle = false;
    }
  }

  // LED and Distance Sensor
  long distance = getDistance();
  handleLEDs(distance);
  delay(500);
}
