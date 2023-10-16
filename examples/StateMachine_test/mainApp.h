// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef MAINAPP_H
#define	MAINAPP_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "StateMachine.h"

#include <Arduino.h> //needed for Serial.println

void mainApp_setup(uint16_t machineStateHandle, xStateMachineEventFlags *flags);
void mainApp_loop();


#endif	/* MAINAPP_H */
