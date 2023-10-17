/*
 * File:   bluetoothControl.c
 * Author: jeu.tonete
 *
 * Created on 20 de Agosto de 2018, 15:17
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef BLUETOOTHCONTROL_H
#define	BLUETOOTHCONTROL_H

#include "StateMachine.h"
#include "remoteAccessCore.h"

void bluetoothControl_RXHandler(char data);
void bluetoothControl_init(uint16_t machineStateHandle, xStateMachineEventFlags *flags);
void bluetoothControl_hardwareConfig(void);
void bluetoothControl_loop(void);
void SendData(unsigned char *data_out, int size);

#endif	/* BLUETOOTHCONTROL_H */

