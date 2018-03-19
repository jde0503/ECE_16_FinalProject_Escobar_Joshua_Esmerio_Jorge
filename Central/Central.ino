/******************************************************************************
* Authors: Joshua Escobar, PID: A11606542
*          Jorge Esmerio, PID: A11459879
* Class: ECE 16
* Lab: Final Project
*
* File Name: Central.ino
* Description: Retrieves commands send from Peripheral Arduino device and
*              controls vibrators and buzzers to convey commands to user.
******************************************************************************/

// Include necessary libraries.
#include <CurieBLE.h>


// Declare and initialize global variables and constants.
const int BASE_PULSE_DURATION = 250; // 250 ms = 1/4 s
const int LEFT_INNER_MOTOR = 4;
const int LEFT_OUTER_MOTOR = 5;
const int RIGHT_INNER_MOTOR = 6;
const int RIGHT_OUTER_MOTOR = 7;
char commandValue;


void setup() {
    // Establish Serial Connection
    Serial.begin(9600);

    // Set up pins.
    pinMode(LEFT_INNER_MOTOR, OUTPUT);
    pinMode(LEFT_OUTER_MOTOR, OUTPUT);
    pinMode(RIGHT_INNER_MOTOR, OUTPUT);
    pinMode(RIGHT_OUTER_MOTOR, OUTPUT);

    // Initialize pin state. HIGH for motors is off.
    digitalWrite(LEFT_INNER_MOTOR, HIGH);
    digitalWrite(LEFT_OUTER_MOTOR, HIGH);
    digitalWrite(RIGHT_INNER_MOTOR, HIGH);
    digitalWrite(RIGHT_OUTER_MOTOR, HIGH);

    //**************************** BLE Code ****************************
    // Initiliaze the BLE hardware
    BLE.begin();

    // If debugging, print status update.
    if (Serial) {
        Serial.println("BLE Central Initialized");
        Serial.println("Central scanning for Peripheral UUID...");
    }
    // Scan/look for a Peripheral
    BLE.scanForUuid("861c36f6-2701-11e8-b467-0ed5f89f718b");
    //******************************************************************
}


void loop() {
    // Check if a peripheral has been discovered.
    BLEDevice peripheral_BLE = BLE.available();

    // If peripheral has been discovered...
    if (peripheral_BLE) {
        // If debugging, print status update.
        if (Serial) {
            Serial.print("Found Peripheral: ");
            Serial.println(peripheral_BLE.localName());
        }

        // Connect to peripheral
        peripheral_BLE.connect();

        // If connected to peripheral...
        if (peripheral_BLE.connected()){
            // If debugging, print status update.
            if (Serial) {
                Serial.println("Periperipheral Connected");
                Serial.println("Stopped Scanning for Peripheral");
            }

            // Stop scanning
            BLE.stopScan();

            // If debugging, print status update.
            if (Serial) {
                Serial.println("Discovering the Peripheral's Attributes");
            }

            // Discover the peripheral's attributes
            peripheral_BLE.discoverAttributes();

            // Peripheral services
            BLEService controller_serv =
            peripheral_BLE.service("861c36f6-2701-11e8-b467-0ed5f89f718b");

            // Service's characteristics
            BLECharacteristic command =
            controller_serv.characteristic("861c3d04-2701-11e8-b467-0ed5f89f718b");

            BLECharacteristic newCommand =
            controller_serv.characteristic("861c4100-2701-11e8-b467-0ed5f89f718b");

            // If debugging, print status update.
            if (Serial) {
                Serial.println("Entering main program loop");
            }

            // MAIN Program
            while (peripheral_BLE.connected()) {
                // Check if central has appropriate permissions.
                if (command.canRead() && newCommand.canRead() &&
                newCommand.canWrite()) {

                    // Check if new command has been issued.
                    // If so, parse command and execute it.
                    newCommand.read();
                    if (newCommand.intValue()) {
                        // Read command.
                        command.read();
                        commandValue = command.charValue();

                        // If debugging, print status update.
                        if (Serial) {
                            Serial.print("Received new commmand: ");
                            Serial.println(commandValue);

                        }

                        switch (commandValue) {
                            case 'F':
                                digitalWrite(LEFT_OUTER_MOTOR, LOW);
                                digitalWrite(RIGHT_OUTER_MOTOR, LOW);
                                delay(BASE_PULSE_DURATION);
                                digitalWrite(LEFT_OUTER_MOTOR, HIGH);
                                digitalWrite(RIGHT_OUTER_MOTOR, HIGH);

                                // Update newCommand characteristic.
                                newCommand.writeInt(0);

                                // If debugging, print status update.
                                if (Serial) {
                                    Serial.println("Walking Forward");
                                }
                            case 'B':
                                digitalWrite(LEFT_INNER_MOTOR, LOW);
                                digitalWrite(RIGHT_INNER_MOTOR, LOW);
                                delay(BASE_PULSE_DURATION);
                                digitalWrite(LEFT_INNER_MOTOR, HIGH);
                                digitalWrite(RIGHT_INNER_MOTOR, HIGH);

                                // Update newCommand characteristic.
                                newCommand.writeInt(0);

                                // If debugging, print status update.
                                if (Serial) {
                                    Serial.println("Walking Backward");
                                }

                            case 'L':
                                digitalWrite(LEFT_INNER_MOTOR, LOW);
                                digitalWrite(LEFT_OUTER_MOTOR, LOW);
                                delay(BASE_PULSE_DURATION);
                                digitalWrite(LEFT_INNER_MOTOR, HIGH);
                                digitalWrite(LEFT_OUTER_MOTOR, HIGH);

                                // Update newCommand characteristic.
                                newCommand.writeInt(0);

                                // If debugging, print status update.
                                if (Serial) {
                                    Serial.println("Turning Left");
                                }

                            case 'R':
                                digitalWrite(RIGHT_INNER_MOTOR, LOW);
                                digitalWrite(RIGHT_OUTER_MOTOR, LOW);
                                delay(BASE_PULSE_DURATION);
                                digitalWrite(RIGHT_INNER_MOTOR, HIGH);
                                digitalWrite(RIGHT_OUTER_MOTOR, HIGH);

                                // Update newCommand characteristic.
                                newCommand.writeInt(0);

                                // If debugging, print status update.
                                if (Serial) {
                                    Serial.println("Turning Right");
                                }

                            case 'U':
                                digitalWrite(LEFT_OUTER_MOTOR, LOW);
                                digitalWrite(RIGHT_OUTER_MOTOR, LOW);
                                delay(BASE_PULSE_DURATION);
                                digitalWrite(LEFT_OUTER_MOTOR, HIGH);
                                digitalWrite(RIGHT_OUTER_MOTOR, HIGH);
                                delay(BASE_PULSE_DURATION * 0.5);
                                digitalWrite(LEFT_OUTER_MOTOR, LOW);
                                digitalWrite(RIGHT_OUTER_MOTOR, LOW);
                                delay(BASE_PULSE_DURATION);
                                digitalWrite(LEFT_OUTER_MOTOR, HIGH);
                                digitalWrite(RIGHT_OUTER_MOTOR, HIGH);

                                // Update newCommand characteristic.
                                newCommand.writeInt(0);

                                // If debugging, print status update.
                                if (Serial) {
                                    Serial.println("Picking Item Up");
                                }

                            case 'D':
                                digitalWrite(LEFT_INNER_MOTOR, LOW);
                                digitalWrite(RIGHT_INNER_MOTOR, LOW);
                                delay(BASE_PULSE_DURATION);
                                digitalWrite(LEFT_INNER_MOTOR, HIGH);
                                digitalWrite(RIGHT_INNER_MOTOR, HIGH);
                                delay(BASE_PULSE_DURATION * 0.5);
                                digitalWrite(LEFT_INNER_MOTOR, LOW);
                                digitalWrite(RIGHT_INNER_MOTOR, LOW);
                                delay(BASE_PULSE_DURATION);
                                digitalWrite(LEFT_INNER_MOTOR, HIGH);
                                digitalWrite(RIGHT_INNER_MOTOR, HIGH);

                                // Update newCommand characteristic.
                                newCommand.writeInt(0);

                                // If debugging, print status update.
                                if (Serial) {
                                    Serial.println("Putting Item Down");
                                }

                            case 'S':
                            default:
                                digitalWrite(LEFT_INNER_MOTOR, LOW);
                                digitalWrite(LEFT_OUTER_MOTOR, LOW);
                                digitalWrite(RIGHT_INNER_MOTOR, LOW);
                                digitalWrite(RIGHT_OUTER_MOTOR, LOW);
                                delay(BASE_PULSE_DURATION * 3);
                                digitalWrite(LEFT_INNER_MOTOR, HIGH);
                                digitalWrite(LEFT_OUTER_MOTOR, HIGH);
                                digitalWrite(RIGHT_INNER_MOTOR, HIGH);
                                digitalWrite(RIGHT_OUTER_MOTOR, HIGH);

                                // Update newCommand characteristic.
                                newCommand.writeInt(0);

                                // If debugging, print status update.
                                if (Serial) {
                                    Serial.println("Stopping");
                                }
                        }
                    }
                    else {
                        // If debugging, print status update.
                        if (Serial) {
                            Serial.println("New command not recieved yet");
                        }
                    }
                }
                else {
                    // If debugging, print status update.
                    if (Serial) {
                        Serial.println("Improper permissions settings detected!");
                    }
                }
            }
        }
        // If debugging, print status update.
        if (Serial) {
            Serial.println("BLE connection Lost. Scanning for peripheral...");
        }

        // Peripheral disconnected, start scanning again
        BLE.scanForUuid("861c36f6-2701-11e8-b467-0ed5f89f718b");
    }
}
