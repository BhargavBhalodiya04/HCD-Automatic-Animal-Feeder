#include <SoftwareSerial.h>

// Motor Pins
const int ENA = 5;     // PWM pin
const int IN1 = 6;     // Direction pin 1
const int IN2 = 7;     // Direction pin 2

// Bluetooth on pins 2 (RX), 3 (TX)
SoftwareSerial mySerial(2, 3);

unsigned long delayStartTime = 0;
unsigned long motorStartTime = 0;
unsigned long delayDuration = 0;
const unsigned long motorRunDuration = 60000UL; // 1 minute
const unsigned long speedReduceDelay = 10;      // 10 ms delay before reducing speed
bool waitingToStart = false;
bool motorRunning = false;
bool speedReduced = false;

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  mySerial.begin(9600);
  Serial.begin(9600);

  mySerial.println("Send 1-9 to start motor after delay (in seconds). Send 0 to stop.");
  Serial.println("Send 1-9 to start motor after delay (in seconds). Send 0 to stop.");
}

void loop() {
  // Read Bluetooth input
  if (mySerial.available()) {
    char input = mySerial.read();

    if (input == '0') {
      stopMotor();
      waitingToStart = false;
      mySerial.println("Motor stopped by user.");
      Serial.println("Motor stopped by user.");
    }
    else if (input >= '1' && input <= '9') {
      int delaySec = input - '0';
      delayDuration = delaySec * 1000UL;
      delayStartTime = millis();
      waitingToStart = true;
      motorRunning = false;
      speedReduced = false;

      mySerial.print("Motor will start after ");
      mySerial.print(delaySec);
      mySerial.println(" seconds.");
      Serial.print("Motor will start after ");
      Serial.print(delaySec);
      Serial.println(" seconds.");
    }
  }

  // Start motor after delay
  if (waitingToStart && (millis() - delayStartTime >= delayDuration)) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 100); // Start at high speed
    motorStartTime = millis();
    motorRunning = true;
    waitingToStart = false;

    mySerial.println("Motor started at high speed.");
    Serial.println("Motor started at high speed.");
  }

  // Reduce speed after 10 ms
  if (motorRunning && !speedReduced && millis() - motorStartTime >= speedReduceDelay) {
    analogWrite(ENA, 40);  // Reduce speed
    speedReduced = true;

    mySerial.println("Motor speed reduced to 40.");
    Serial.println("Motor speed reduced to 40.");
  }

  // Stop motor after 1 minute
  if (motorRunning && millis() - motorStartTime >= motorRunDuration) {
    stopMotor();
    mySerial.println("Motor stopped after 1 minute.");
    Serial.println("Motor stopped after 1 minute.");
  }
}

void stopMotor() {
  analogWrite(ENA, 0);   // Stop motor
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  motorRunning = false;
  speedReduced = false;
}


// // Motor A Pins
// const int ENA = 5;
// const int IN1 = 6;
// const int IN2 = 7;

// // Motor B Pins
// const int ENB = 10;
// const int IN3 = 8;
// const int IN4 = 9;

// void setup() {
//   // Set motor pins as output
//   pinMode(ENA, OUTPUT);
//   pinMode(IN1, OUTPUT);
//   pinMode(IN2, OUTPUT);

//   pinMode(ENB, OUTPUT);
//   pinMode(IN3, OUTPUT);
//   pinMode(IN4, OUTPUT);
// }

// void loop() {
//   // Rotate Motor A Forward
//   digitalWrite(IN1, HIGH);
//   digitalWrite(IN2, LOW);
//   analogWrite(ENA, 150); // speed 0-255

//   // Rotate Motor B Backward
//   digitalWrite(IN3, LOW);
//   digitalWrite(IN4, HIGH);
//   analogWrite(ENB, 150); // speed 0-255

//   delay(2000); // run motors for 2 seconds

//   // Stop both motors
//   digitalWrite(IN1, LOW);
//   digitalWrite(IN2, LOW);
//   digitalWrite(IN3, LOW);
//   digitalWrite(IN4, LOW);
//   analogWrite(ENA, 0);
//   analogWrite(ENB, 0);

//   delay(2000); // wait for 2 seconds
// }
