/******************************************************************************
* Author: Joshua Escobar, PID: A11606542
* Class:  ECE 16
* Lab: Final Project
*
* File Name: Central.ino
* Description: Retrieves commands send from Peripheral Arduino device and
*              controls vibrators and buzzers to convey commands to user.
******************************************************************************/

// Include necessary libraries.
#include <CurieBLE.h>

void setup() {
  // Establish Serial Connection
  Serial.begin(9600);
  // Wait for connection to establish
  while(!Serial);
  Serial.println("Serial Established.");
  //**************************** BLE Code ****************************
  // Initiliaze the BLE hardware
  BLE.begin();
  Serial.println("BLE Central Initialized");
  // Scan/look for a Peripheral
  Serial.println("Central scanning for Peripheral UUID...");
  BLE.scanForUuid("861c398a-2701-11e8-b467-0ed5f89f718b");
  //******************************************************************
}

void loop() {
  // check if a peripheral has been discovered
  BLEDevice peripheral_BLE = BLE.available();
  if (peripheral_BLE) {
    // Discovered a peripheral, local name, and advertised service
    Serial.print("Found Peripheral: ");
    Serial.println(peripheral_BLE.localName());
    // Connect to peripheral
    peripheral_BLE.connect();


    // If connected to peripheral
    if (peripheral_BLE.connected()){
      Serial.println("Periperipheral Connected");
      // Stop scanning
      Serial.println("Stopped Scanning for Peripheral");
      BLE.stopScan();

      // Discover the peripheral's attributes
      Serial.println("Discovering the Peripheral's Attributes");
      peripheral_BLE.discoverAttributes();
      if (Serial) {
        Serial.print("Service Count: ");
        Serial.println(peripheral_BLE.serviceCount());
      }
      // Peripheral services
      BLEService accel_serv = peripheral_BLE.service("861c36f6-2701-11e8-b467-0ed5f89f718b");
      BLEService gyro_serv = peripheral_BLE.service("861c398a-2701-11e8-b467-0ed5f89f718b");
      BLEService emg_serv = peripheral_BLE.service("861c3b6a-2701-11e8-b467-0ed5f89f718b");
      if (Serial) {
        Serial.print("Accel Char Count: ");
        Serial.print(accel_serv.characteristicCount());
        Serial.print("\t");

        Serial.print("Gyro Char Count: ");
        Serial.print(gyro_serv.characteristicCount());
        Serial.print("\t");

        Serial.print("EMG Char Count: ");
        Serial.println(emg_serv.characteristicCount());
      }

      // Services' characteristics
      BLECharacteristic ax = accel_serv.characteristic("861c3d04-2701-11e8-b467-0ed5f89f718b");
      BLECharacteristic ay = accel_serv.characteristic("861c4100-2701-11e8-b467-0ed5f89f718b");
      BLECharacteristic az = accel_serv.characteristic("861c4254-2701-11e8-b467-0ed5f89f718b");
      BLECharacteristic gx = gyro_serv.characteristic("861c43ee-2701-11e8-b467-0ed5f89f718b");
      BLECharacteristic gy = gyro_serv.characteristic("861c45a6-2701-11e8-b467-0ed5f89f718b");
      BLECharacteristic gz = gyro_serv.characteristic("861c46dc-2701-11e8-b467-0ed5f89f718b");
      BLECharacteristic emg_1 = emg_serv.characteristic("861c49d4-2701-11e8-b467-0ed5f89f718b");
      BLECharacteristic emg_2 = emg_serv.characteristic("861c4b5a-2701-11e8-b467-0ed5f89f718b");

      // MAIN Program
      while(peripheral_BLE.connected()){
        // Check if central has read permision
        if (ax.canRead() && ay.canRead() && az.canRead() && gx.canRead() && gy.canRead() && gz.canRead()
            && emg_1.canRead() && emg_2.canRead()){
          // Check if new value was written to characteristic
          if (true){//ax.written() && ay.written() && az.written()){
            // Read characteristic values
            ax.read(); ay.read(); az.read();
            // Display data
            Serial.print("A_x: ");
            Serial.print(ax.floatValue());
            Serial.print("\t");
            Serial.print("A_y: ");
            Serial.print(ay.floatValue());
            Serial.print("\t");
            Serial.print("A_z: ");
            Serial.println(az.floatValue());
          }
          if (true){//gx.written() && gy.written() && gz.written()){
            // Read characteristic values
            gx.read(); gy.read(); gz.read();
            // Display data
            Serial.print("G_x: ");
            Serial.print(gx.floatValue());
            Serial.print("\t");
            Serial.print("G_y: ");
            Serial.print(gy.floatValue());
            Serial.print("\t");
            Serial.print("G_z: ");
            Serial.println(gz.floatValue());
          }
          if (true){//emg_1.written() && emg_2.written()){
            // Read characteristic values
            emg_1.read(); emg_2.read();
            // Display data
            Serial.print("EMG 1: ");
            Serial.print(emg_1.intValue());
            Serial.print("\t");
            Serial.print("EMG 2: ");
            Serial.println(emg_2.intValue());
          }
          else{
            Serial.println("New value not written yet");
          }
        }
        else{
          Serial.println("No READ Permission for ax, ay, or az");
        }
      }
    }
    // Peripheral disconnected, start scanning again
    BLE.scanForUuid("861c36f6-2701-11e8-b467-0ed5f89f718b");
  }
}
