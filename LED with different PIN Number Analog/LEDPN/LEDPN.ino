// #define TRIG_PIN 12
// #define ECHO_PIN 11
// #define RED_LED A0
// #define BLUE_LED A1
// #define GREEN_LED A2

// void setup() {
//     pinMode(TRIG_PIN, OUTPUT);
//     pinMode(ECHO_PIN, INPUT);
//     pinMode(RED_LED, OUTPUT);
//     pinMode(BLUE_LED, OUTPUT);
//     pinMode(GREEN_LED, OUTPUT);
//     Serial.begin(9600);
// }

// long getDistance() {
//     digitalWrite(TRIG_PIN, LOW);
//     delayMicroseconds(2);
//     digitalWrite(TRIG_PIN, HIGH);
//     delayMicroseconds(10);
//     digitalWrite(TRIG_PIN, LOW);
    
//     long duration = pulseIn(ECHO_PIN, HIGH);
//     long distance = duration * 0.034 / 2;  // Convert to cm
//     return distance;
// }

// void loop() {
//     long distance = getDistance();
//     Serial.print("Distance: ");
//     Serial.print(distance);
//     Serial.println(" cm");

//     // Ensure only ONE LED is ON at a time
//     if (distance <= 10) {
//         digitalWrite(RED_LED, HIGH);
//         digitalWrite(BLUE_LED, LOW);
//         digitalWrite(GREEN_LED, LOW);
//     } else if (distance >= 11 && distance <= 30) {
//         digitalWrite(RED_LED, LOW);
//         digitalWrite(BLUE_LED, HIGH);
//         digitalWrite(GREEN_LED, LOW);
//     } else if (distance >= 31 && distance <= 100) {
//         digitalWrite(RED_LED, LOW);
//         digitalWrite(BLUE_LED, LOW);
//         digitalWrite(GREEN_LED, HIGH);
//     } else {
//         // Turn off all LEDs if distance is out of range
//         digitalWrite(RED_LED, LOW);
//         digitalWrite(BLUE_LED, LOW);
//         digitalWrite(GREEN_LED, LOW);
//     }

//     delay(500); // Delay for stability
// }`                                   
#include <SoftwareSerial.h>

SoftwareSerial bluetooth(2, 3); // RX, TX

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  Serial.println("Bluetooth Echo Ready.");
}

void loop() {
  // If something comes from Bluetooth
  if (bluetooth.available()) {
    char c = bluetooth.read();
    Serial.print("BT: ");
    Serial.println(c);         // Show in Serial Monitor
    bluetooth.print("You sent: ");
    bluetooth.println(c);      // Echo back to phone
  }

  // If something is typed into Serial Monitor, send to Bluetooth
  if (Serial.available()) {
    char c = Serial.read();
    bluetooth.write(c);
  }
}
