/******************************************************************************
* Author: Joshua Escobar, PID: A11606542
* Class:  ECE 16
* Lab: Final Project
*
* File Name: Peripherall.ino
* Description: Continuosly samples EMG and IMU data, processes the data and
*              interprets it as a specific command, transmitting that command
*              to the central Arduino.
******************************************************************************/

// Include necessary libraries.
#include <CurieBLE.h>
#include <CurieIMU.h>
#include "CurieTimerOne.h"

// Global variables
volatile bool hasRead = false;   // toggle to print
float ax, ay, az, gx, gy, gz;    // accel/gyro values
int EMG_data1, EMG_data2;                    // EMG value

//************************************** BLE Code ****************************************
//BLEPeripheral my_BLE_Periph;  // BLE Peripheral Device (the board you're programming)
BLEService accel_service("861c36f6-2701-11e8-b467-0ed5f89f718b"); // BLE ACCEL Service
BLEService gyro_service("861c398a-2701-11e8-b467-0ed5f89f718b");   // BLE GYRO Service
BLEService emg_service("861c3b6a-2701-11e8-b467-0ed5f89f718b");     // BLE EMG Service
// Set up characteristics
// BLE Accelerometer Service Characteristics
BLEFloatCharacteristic accel_characteristic_x("861c3d04-2701-11e8-b467-0ed5f89f718b", BLERead | BLEWrite);
BLEFloatCharacteristic accel_characteristic_y("861c4100-2701-11e8-b467-0ed5f89f718b", BLERead | BLEWrite);
BLEFloatCharacteristic accel_characteristic_z("861c4254-2701-11e8-b467-0ed5f89f718b", BLERead | BLEWrite);
// BLE Gyroscope Service Characteristics
BLEFloatCharacteristic gyro_characteristic_x("861c43ee-2701-11e8-b467-0ed5f89f718b", BLERead | BLEWrite);
BLEFloatCharacteristic gyro_characteristic_y("861c45a6-2701-11e8-b467-0ed5f89f718b", BLERead | BLEWrite);
BLEFloatCharacteristic gyro_characteristic_z("861c46dc-2701-11e8-b467-0ed5f89f718b", BLERead | BLEWrite);
// BLE EMG Service Characteristics
BLEIntCharacteristic emg_characteristic_1("861c49d4-2701-11e8-b467-0ed5f89f718b", BLERead | BLEWrite);
BLEIntCharacteristic emg_characteristic_2("861c4b5a-2701-11e8-b467-0ed5f89f718b", BLERead | BLEWrite);
//****************************************************************************************

void setup() {
  // Establish Serial Connection
  Serial.begin(9600);
  // Wait for connection to establish
  //while(!Serial);
  //Serial.println("Serial Established.");
  // Initialize IMU
  CurieIMU.begin();
  CurieIMU.setGyroRange(250);
  CurieIMU.setAccelerometerRange(2);
  //Serial.println("IMU Initialized.");

  //**************************** BLE Code ****************************
  // Initiliaze the BLE hardware
  BLE.begin();
  // set advertised local name and service UUID:
  BLE.setLocalName("Josh & Jorge BLE");
  BLE.setAdvertisedServiceUuid(gyro_service.uuid());
  // add characteristics to services
  accel_service.addCharacteristic(accel_characteristic_x);
  accel_service.addCharacteristic(accel_characteristic_y);
  accel_service.addCharacteristic(accel_characteristic_z);
  gyro_service.addCharacteristic(gyro_characteristic_x);
  gyro_service.addCharacteristic(gyro_characteristic_y);
  gyro_service.addCharacteristic(gyro_characteristic_z);
  emg_service.addCharacteristic(emg_characteristic_1);
  emg_service.addCharacteristic(emg_characteristic_2);
  // add service to peripheral
  BLE.addService(accel_service);
  BLE.addService(gyro_service);
  BLE.addService(emg_service);
  // set initial characteristic values
  accel_characteristic_x.writeFloat(float(1.0));
  accel_characteristic_y.writeFloat(float(1.0));
  accel_characteristic_z.writeFloat(float(1.0));
  gyro_characteristic_x.writeFloat(float(1.0));
  gyro_characteristic_y.writeFloat(float(1.0));
  gyro_characteristic_z.writeFloat(float(1.0));
  emg_characteristic_1.writeInt(1);
  emg_characteristic_2.writeInt(1);
  // BEGIN ADVERTISING THE BLE
  //Serial.println("BLE being advertised");
  BLE.advertise();
  //******************************************************************

  // Start Interupt, at 10Hz
  CurieTimerOne.start(100000, &my_sample);
  //Serial.println("Interupt Started.");
}

// loop() function used to establish and reestablish BLE connection
void loop() {
  //******************************* BLE Code *******************************
  // look for a BLE central to connect to
  BLEDevice central_BLE = BLE.central();
  // if connection established between Peripheral and Central
  if (central_BLE) {
    //Serial.print("Connected to central: ");
    // Stop advertising BLE
    BLE.stopAdvertise();
    //Serial.println("Stopped advertising BLE ");

    // while the Peripheral is still connected to Central, write data
    while (central_BLE.connected()) {
      // write to characteristics, if sampled
      if (hasRead) {
        accel_characteristic_x.writeFloat(ax);
        accel_characteristic_y.writeFloat(ay);
        accel_characteristic_z.writeFloat(az);
        gyro_characteristic_x.writeFloat(gx);
        gyro_characteristic_y.writeFloat(gy);
        gyro_characteristic_z.writeFloat(gz);
        emg_characteristic_1.writeInt(EMG_data1);
        emg_characteristic_2.writeInt(EMG_data2);
        hasRead = false;

        if (Serial) {
            Serial.print("A_x: ");
            Serial.print(accel_characteristic_x.floatValue());
            Serial.print("\t");
            Serial.print("A_y: ");
            Serial.print(accel_characteristic_y.floatValue());
            Serial.print("\t");
            Serial.print("A_z: ");
            Serial.println(accel_characteristic_z.floatValue());

            Serial.print("G_x: ");
            Serial.print(gyro_characteristic_x.floatValue());
            Serial.print("\t");
            Serial.print("G_y: ");
            Serial.print(gyro_characteristic_y.floatValue());
            Serial.print("\t");
            Serial.print("G_z: ");
            Serial.println(gyro_characteristic_z.floatValue());

            Serial.print("EMG 1: ");
            Serial.print(emg_characteristic_1.intValue());
            Serial.print("\t");
            Serial.print("EMG 2: ");
            Serial.println(emg_characteristic_2.intValue());
        }
      }
    }
    // When the central disconnects, print it out:
    //Serial.println("Disconnected from central");
    // Advertise BLE again
    //Serial.println("BLE being advertised");
    BLE.advertise();
  }
  //************************************************************************
}


// Sampling function (ISR)
void my_sample(){
  // Sample Accel, Gyro, then EMG
  CurieIMU.readAccelerometerScaled(ax, ay, az);
  CurieIMU.readGyroScaled(gx, gy, gz);
  EMG_data1 = analogRead(A0);
  EMG_data2 = analogRead(A1);
  // Toggle print ON
  hasRead = true;
}
