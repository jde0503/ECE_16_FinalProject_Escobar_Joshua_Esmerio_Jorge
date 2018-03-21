/*****************************************************************************
* Authors: Joshua Escobar, PID: A11606542
*          Jorge Esmerio, PID: A11459879
* Class:  ECE 16
* Lab: Final Project
*
* File Name: Peripheral-Python.ino
* Description: Continuosly samples EMG and IMU data, sending it via Serial to
               Python script for processing. Updates BLE characterisitcs with
               commands sent back from Python.
******************************************************************************/

// Include necessary libraries.
#include <CurieBLE.h>
#include "CurieIMU.h"
#include "CurieTimerOne.h"


// Declare and initialize global variables and constants.
const int BAUD_RATE = 115200;
const int SAMPLE_PERIOD = 6666; // 6.66 millisec -> 150 Hz
const int LEFT_CALF_PIN = A0;
const int RIGHT_CALF_PIN = A1;
int A_x, A_y, A_z;
float leftCalfRaw, rightCalfRaw;
float leftCalfScaled, rightCalfScaled;
volatile bool newSample = false;

BLEService commandService("861c36f6-2701-11e8-b467-0ed5f89f718b");

BLECharCharacteristic command("861c3d04-2701-11e8-b467-0ed5f89f718b", BLERead | BLEWrite);
BLEIntCharacteristic newCommandStatus("861c4100-2701-11e8-b467-0ed5f89f718b", BLERead | BLEWrite);


// ISR to sample sensors.
void sample() {
    leftCalfRaw = analogRead(LEFT_CALF_PIN);
    rightCalfRaw = analogRead(RIGHT_CALF_PIN);
    CurieIMU.readAccelerometer(A_x, A_y, A_z);
    newSample = true;
}


void setup() {
    // Initialize Serial
    Serial.begin(BAUD_RATE);
    while(!Serial) {} // Wait for Serial port to open.

    // Initialize accelerometer sensor.
    CurieIMU.begin();
    CurieIMU.setAccelerometerRange(2);

    // Initialize BLE
    BLE.begin();
    BLE.setLocalName("Josh & Jorge Final Project");
    BLE.setAdvertisedServiceUuid(commandService.uuid());
    final_service.addCharacteristic(command);
    final_service.addCharacteristic(newCommandStatus);
    BLE.addService(commandService);
    final_command.writeChar('S');
    new_command.writeInt(0);
    BLE.advertise();

    // Begin sampling
    CurieTimerOne.start(SAMPLE_PERIOD, &sample);
}


void loop() {
    BLEDevice centralBLE = BLE.central();

    // If connection established between Peripheral and Central...
    if (centralBLE) {

        // Stop advertising BLE
        BLE.stopAdvertise();

        // While the Peripheral is still connected to Central...
        while (centralBLE.connected()) {
            // Check to see if latest command has been received from Central
            // and check if theres a new command from python...
            newCommandStatus.read();
            if (Serial.available() && !newCommandStatus.intValue()) {
                // Update BLE characterisitcs.
                command.writeChar(Serial.read());
                newCommandStatus.writeInt(1);
            }

            // Send any new sensor data to python via serial.
            if (newSample) {
                // Scale EMG readings before transmission.
                leftCalfScaled = ((float(map(LeftCalfRaw, 0, 1023, 0, 33)) *
                                 100.0) - 1500) / (2800.0);
                rightCalfScaled = ((float(map(rightCalfRaw, 0, 1023, 0, 33)) *
                                 100.0) - 1500) / (2800.0);

                // Send data.
                Serial.print(A_x);
                Serial.print("\t");
                Serial.print(A_y);
                Serial.print("\t");
                Serial.print(A_z);
                Serial.print("\t");
                Serial.print(leftCalfScaled);
                Serial.print("\t");
                Serial.println(rightCalfScaled);
            }
        }
        // Advertise BLE again
        BLE.advertise();
    }
}
