#include <SoftwareSerial.h>

// Bluetooth Module Pins
#define TxD 2
#define RxD 3
SoftwareSerial mySerial(RxD, TxD); // RX, TX for Bluetooth

// Motor 1 Pins
int ena = 5;
int in1 = 6;
int in2 = 7;

// Motor 2 Pins
int enb = 10;
int in3 = 8;
int in4 = 9;

// Motor 3 Pins
const int ENA = 11;
const int IN1 = 12;
const int IN2 = 13;

unsigned long startTime;
bool speedReduced = false;
bool motorStopped = false;

int delayTime = 3; // default in seconds

// For motors 1 and 2
unsigned long lastCycleTime = 0;
bool motorsRunning = false;
bool motorsStopped = true;

void setup() {
    mySerial.begin(9600);
    Serial.begin(9600);

    pinMode(ena, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(enb, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);

    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);

    // Motor 3 setup
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 100);
    startTime = millis();

    Serial.println("Ready! Enter a number (1-9) via Bluetooth.");
    mySerial.println("Enter a number (1-9) to set delay time.");
}

void loop() {
    unsigned long currentTime = millis();

    // Bluetooth input for delay time
    if (mySerial.available()) {
        char c = mySerial.read();
        while (mySerial.available()) mySerial.read();
        if (c >= '1' && c <= '9') {
            delayTime = c - '0';
            Serial.print("Updated delay time: ");
            Serial.print(delayTime);
            Serial.println(" seconds");
            mySerial.print("Updated delay time: ");
            mySerial.print(delayTime);
            mySerial.println(" seconds");
        }
    }

    // Motor 3 speed reduction after 10ms
    if (!speedReduced && currentTime - startTime >= 10) {
        analogWrite(ENA, 40);
        speedReduced = true;
        Serial.println("Motor 3 speed reduced after 10ms");
    }

    // Motor 3 stop after 60 seconds
    if (!motorStopped && currentTime - startTime >= 60000) {
        analogWrite(ENA, 0);
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        motorStopped = true;
        Serial.println("Motor 3 stopped after 1 minute");
    }

    // Non-blocking motor 1 and 2 control
    if (motorsStopped && currentTime - lastCycleTime >= delayTime * 1000) {
        Serial.println("Rotating Motors...");
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        analogWrite(ena, 100);

        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
        analogWrite(enb, 100);

        motorsRunning = true;
        motorsStopped = false;
        lastCycleTime = currentTime;
    }

    if (motorsRunning && currentTime - lastCycleTime >= 1000) { // Run for 1 sec
        Serial.println("Stopping Motors...");
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        analogWrite(ena, 0);

        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);
        analogWrite(enb, 0);

        motorsRunning = false;
        motorsStopped = true;
        lastCycleTime = currentTime;
        Serial.println("Waiting before next cycle...");
    }
}
