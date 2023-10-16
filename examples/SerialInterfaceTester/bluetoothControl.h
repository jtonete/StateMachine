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

#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC33E__)
    	#include <p33Exxxx.h>
    #elif defined(__dsPIC33F__)
    	#include <p33Fxxxx.h>
    #endif
#endif
//#include "system.h"
#include "StateMachine.h"
#include "remoteAccessCore.h"
//#include "UART1.h"

void bluetoothControl_RXHandler(char data);
void bluetoothControl_init(uint16_t machineStateHandle, xStateMachineEventFlags *flags);
void bluetoothControl_hardwareConfig(void);
void bluetoothControl_loop(void);
void SendData(unsigned char *data_out, int size);

#endif	/* BLUETOOTHCONTROL_H */

