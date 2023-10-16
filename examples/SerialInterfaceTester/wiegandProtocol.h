/*
 * File:   wiegandProtocol.h
 * Author: jeu.tonete
 *
 * Created on 2021.06
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef WIEGANDPROTOCOL_H
#define	WIEGANDPROTOCOL_H

#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC33E__)
    	#include <p33Exxxx.h>
    #elif defined(__dsPIC33F__)
    	#include <p33Fxxxx.h>
    #endif
#endif

#include "StateMachine.h"

typedef struct wiegand { //
    uint8_t data[8];
    uint8_t dataSizeInBytes;
    bool SendData;
    bool ReadyToSend;
} Wiegand;


void wiegandProtocol_RXHandler(char data);
void wiegandProtocol_init(uint16_t machineStateHandle, xStateMachineEventFlags *flags);
void wiegandProtocol_hardwareConfig(void);
void wiegandProtocol_loop(void);

void wiegandProtocol_SetDataToSend(void);
bool wiegandProtocol_IsReadyToSend(void);


#endif	/* WIEGANDPROTOCOL_H */

