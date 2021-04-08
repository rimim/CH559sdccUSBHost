# CH559sdccUSBHost


Forked from https://github.com/atc1441/CH559sdccUSBHost

This updated version is used to pair a PS3 controller to a fake Bluetooth address. The BT address can be defined in the btmac.h header file or automatically generated at compile time. The CH559 will print this BT address to the serial port at startup. The CH559 is connected to an ESP32 that reads this address and sets its own BT address to this address. The PS3 controller will then attempt to pair with the ESP32.

The relevant functions in USBHost.c are getHIDDeviceReport() and pairDevice().

