typedef unsigned char *PUINT8;
typedef unsigned char __xdata *PUINT8X;
typedef const unsigned char __code *PUINT8C;
typedef unsigned char __xdata UINT8X;
typedef unsigned char  __data             UINT8D;

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "CH559.h"
#include "util.h"
#include "USBHost.h"
#include "uart.h"
#include "btmac.h"

//__at (DATA_FLASH_ADDR) UINT8PV  pMacFlash[6];

SBIT(LED, 0x90, 6);

void resetInit()
{
    resetHubDevices(0);
    resetHubDevices(1);
    initUSB_Host();
    DEBUG_OUT("Ready\n");
    sendProtocolMSG(MSG_TYPE_STARTUP,0, 0x00, 0x00, 0x00, 0);
}

uint8_t WriteDataFlash(uint16_t Address, uint16_t Data);

void sendBluetoothMac()
{
    // // Read UUID
    unsigned char __xdata mac[6];
    mac[0] = BT_MAC[0];
    mac[1] = BT_MAC[1];
    mac[2] = BT_MAC[2];
    mac[3] = BT_MAC[3];
    mac[4] = BT_MAC[4];
    mac[5] = BT_MAC[5];
    // mac[0] = *(__code uint8_t*)(DATA_FLASH_ADDR+0);
    // mac[1] = *(__code uint8_t*)(DATA_FLASH_ADDR+1);
    // mac[2] = *(__code uint8_t*)(DATA_FLASH_ADDR+2);
    // mac[3] = *(__code uint8_t*)(DATA_FLASH_ADDR+3);
    // mac[4] = *(__code uint8_t*)(DATA_FLASH_ADDR+4);
    // mac[5] = *(__code uint8_t*)(DATA_FLASH_ADDR+5);

    sendProtocolMSG(MSG_TYPE_BTADDRESS, 6, 0x00, 0x00, 0x00, mac);
}

void main()
{
    unsigned char s;

    // first check the boot loader
    if(!(P4_IN & (1 << 6)))
        runBootloader();

    initClock();
    initUART0(1000000, 1);
    // initUART0(9600, 1);

    delay(1000);
    DEBUG_OUT("Startup\n");

    resetInit();

    sendBluetoothMac();
    while(1)
    {
        if(!(P4_IN & (1 << 6)))
            runBootloader();
        processUart();
        s = checkRootHubConnections();
        pollHIDdevice();
    }
}
