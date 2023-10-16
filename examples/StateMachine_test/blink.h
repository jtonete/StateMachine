// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef BLINK_H
#define	BLINK_H

#include <arduino.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "StateMachine.h"
#include <string.h> //needed for memcpy

void blink_setup(uint16_t machineStateHandle, xStateMachineEventFlags *flags);
void blink_st0();
void blink_st1();


#endif	/* BLINK_H */

