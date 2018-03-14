/**********Characteristic Types**********
BLECharacteristic yourCharacteristicName(UUID, properties, maxLen)
BLEBoolCharacteristic ...
BLECharCharacteristic ...
BLEUnsignedCharCharacteristic ...
BLEShortCharacteristic ...
BLEUnsignedShortCharacteristic ...
BLEIntCharacteristic ...
BLEUnsignedIntCharacteristic ...
BLELongCharacteristic ...
BLEUnsignedLongCharacteristic ...
BLEFloatCharacteristic ...
BLEDoubleCharacteristic ...
*/***************************************

/**********Write to Characteristic**********
characteristic_name.canWrite()    // check if we have "write" permission 

// These write to characteristic
characteristic_name.writeString(const String& s);
characteristic_name.writeString(const char* s);
characteristic_name.writeChar(char c);
characteristic_name.writeUnsignedChar(unsigned char c);
characteristic_name.writeByte(byte b);
characteristic_name.writeShort(short s);
characteristic_name.writeUnsignedShort(unsigned short s);
characteristic_name.writeInt(int i);
characteristic_name.writeUnsignedInt(unsigned int i);
characteristic_name.writeLong(long l);
characteristic_name.writeUnsignedLong(unsigned int l);
characteristic_name.writeFloat(float f);
characteristic_name.writeDouble(double d);
*/******************************************

/**********Read from Characteristic**********
characteristic_name.written()  // chack if new value has been written to characteristic
valueUpdated()

characteristic_name.canRead()  // check if we have "read" permission 
characteristic_name.read()     // read from characteristic, doesn't return the actual value



*/******************************************


/*******************Other*******************
BLE.begin()          // Initiliaze the BLE hardware
BLE.end()            // Deinitiliaze the BLE hardware

BLE.connected()      // Is the device connected with another BLE device.
BLE.disconnect()     // Disconnect the connected device/s.



// *** IN PERIPHERAL *** //
BLE.advertise()      // Construct the ADV data and start send advertisement
BLE.stopAdvertise()  // Stop send advertisement
BLEDevice central_BLE = BLE.central()           // Get currently connected central

// *** IN CENTRAL *** //
BLE.scan()                     // Start scanning for peripherals
OR
BLE.scanForUuid("some string UUID")
BLE.scanForName("some string Name")
BLE.scanForAddress("some string Address")

BLEDevice peripheral_BLE = BLE.available();     // Retrieve a discovered peripheral
OR?
BLEDevice peripheral_BLE = BLE.peripheral()     // Get currently connected peripheral

peripheral_BLE.connect()                        // Connect to the peripheral
BLE.stopScan()                                  // Stop scanning for peripherals

peripheral_BLE.discoverAttributes();            // discover the peripheral's attributes
BLEService periph_serv = peripheral_BLE.service("UUID" or int);           // 
hasCharacteristic("UUID" or int)    // Does the device have characteristic
BLECharacteristic serv_char = periph_serv.characteristic("UUID" or int);  // Return the characteristic
periph_serv.characteristicCount()   // Number of characteristics in service
*/******************************************

 
