/*
 * File:   bluetoothControl.c
 * Author: jeu.tonete
 *
 * Created on 20 de Agosto de 2018, 15:17
 */

#include "bluetoothControl.h"
#include "HardwareSerial.h"

remoteAccessObj rxHandlerBluetooth __attribute__((packed));
uint16_t bluetooth_MShandle __attribute__((packed));
xStateMachineEventFlags *bluetooth_MSeventflags __attribute__((packed));

void bluetoothControl_RXHandler(char data)
{
    remoteAccessCore_RXHandlerFromISR(&rxHandlerBluetooth, data);
}

void bluetoothControl_init(uint16_t machineStateHandle, xStateMachineEventFlags *flags)
{
    bluetooth_MShandle = machineStateHandle;
    bluetooth_MSeventflags = flags;
    remoteAccessCore_EventflagConfig(flags);
    vSM_setNextStateIdx(bluetooth_MShandle, bluetoothControl_hardwareConfig, 500);
}

void bluetoothControl_hardwareConfig(void)
{
    memset(&rxHandlerBluetooth, 0, sizeof(remoteAccessObj));                // clear struct

    rxHandlerBluetooth.pSendData = SendData;     // Set Serial.write as output port from  rxHandlerBluetooth
    vSM_setNextStateIdx(bluetooth_MShandle, bluetoothControl_loop, 500);
}

void bluetoothControl_loop(void)
{
    if(rxHandlerBluetooth.rxready == true)      // if a valid frame from bluetooth was received
    {
        remoteAccessCore_ParseReceivedData(&rxHandlerBluetooth);   // Parses the received data from bluetooh
        rxHandlerBluetooth.rxready = 0;
    }
}

void serialEvent()
{
  while (Serial.available()) {
    bluetoothControl_RXHandler(Serial.read());
  }
}

void SendData(unsigned char *data_out, int size)
{
    Serial.write(data_out, size);
}