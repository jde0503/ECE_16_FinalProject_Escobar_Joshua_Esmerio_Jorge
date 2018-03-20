/*Libraries for sampling and acelerometer*/
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
#include "CurieIMU.h"
#include "CurieTimerOne.h"

/*Variables to calculate orientation*/
int baud_rate = 115200;
int timer = 100000;
int x, y, z;
int left_calf, right_calf;
char new_instruction, old_instruction;

BLEService final_service("861c36f6-2701-11e8-b467-0ed5f89f718b");

BLECharCharacteristic final_command("861c3d04-2701-11e8-b467-0ed5f89f718b", BLERead | BLEWrite);
BLEIntCharacteristic new_command("861c4100-2701-11e8-b467-0ed5f89f718b", BLERead | BLEWrite);

void read_instruction(){
  /*Reads from accelerometer sensor*/
  CurieIMU.readAccelerometer(x, y, z);

  /*Read EMG data*/
  left_calf = analogRead(A0);
  right_calf = analogRead(A1);
}

void setup() {
  /*Serial using 115200 baud rate*/
  Serial.begin(baud_rate);
  while(!Serial){}

  /*Inilization accelerometer sensor*/
  CurieIMU.begin();
  CurieIMU.setAccelerometerRange(2);

  BLE.begin();
  BLE.setLocalName("Josh & Jorge Final Project");
  BLE.setAdvertisedServiceUuid(final_service.uuid());
  final_service.addCharacteristic(final_command);
  final_service.addCharacteristic(new_command);
  BLE.addService(final_service);
  final_command.writeChar('S');
  new_command.writeInt(1);
  Serial.println("BLE being advertised");
  BLE.advertise();
  
  /*Begin sampling*/
  CurieTimerOne.start(timer, &read_instruction);
  Serial.println("Interupt Started.");
}

void loop() {
  BLEDevice central_BLE = BLE.central();
  
  // if connection established between Peripheral and Central
  if (central_BLE) {
    
    Serial.print("Connected to central: ");
    
    // Stop advertising BLE
    BLE.stopAdvertise();
    Serial.println("Stopped advertising BLE ");

    // while the Peripheral is still connected to Central, write data
    while (central_BLE.connected()) {
      
      if( new_command.intValue()==0 ){
    
        /*Pick up item*/
        if( (x<-14500) )
          new_instruction = 'U';

        /*Go Forward*/
        if(x>14500)
          new_instruction = 'F';

        /*Turn Right*/
        if(y>14500)
          new_instruction = 'R';

        /*Turn Left*/
        if(y<-14500)
          new_instruction = 'L';

        /*Stop*/
        if(z>14500)
          new_instruction = 'S';

        /*Drop Item*/
        if(z<-14500)
          new_instruction = 'D';

        /*Slow down*/
        if( (left_calf>800)  && (old_instruction=='F') )
          new_instruction = 'B';

        /*Speed Up*/
        if( (right_calf>800)  && (old_instruction=='F') )
          new_instruction = 'F';

        new_command.writeInt(1);
      }
  

      /*If orientation has changed it prints to serial*/
      if(new_instruction != old_instruction){
         final_command.writeChar(new_instruction);   
      }

      /*Saves current orientation to compare to orientation from next sample*/
      old_instruction = new_instruction;
    }
    //When the central disconnects, print it out:
    Serial.println("Disconnected from central");
    // Advertise BLE again
    Serial.println("BLE being advertised");
    BLE.advertise();
  }
}
