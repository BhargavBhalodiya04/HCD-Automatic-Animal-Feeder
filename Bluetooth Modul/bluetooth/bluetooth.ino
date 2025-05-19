#include <SoftwareSerial.h>

// Default Bluetooth Pins (Use if HC-05 is connected to digital pins)
#define BT_TX 2
#define BT_RX 3

SoftwareSerial bluetooth(BT_RX, BT_TX); // RX, TX

void setup() {
    Serial.begin(9600);      // Serial Monitor
    bluetooth.begin(9600);   // Bluetooth Module Baud Rate

    Serial.println("Bluetooth Ready!");
    bluetooth.println("Arduino Bluetooth Ready!");
}

void loop() {
    // Read data from Bluetooth and print to Serial Monitor
    if (bluetooth.available()) {
        String receivedData = bluetooth.readString();
        Serial.print("Received from Bluetooth: ");
        Serial.println(receivedData);
    }

    // Read data from Serial Monitor and send to Bluetooth
    if (bluetooth.available()) {
    String receivedData = bluetooth.readString();
    receivedData.trim();  // Remove unwanted newline characters
    Serial.print("Received from Bluetooth: ");
    Serial.println(receivedData);
}

}
