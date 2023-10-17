/*
 * File:   productCtrl.h
 * Author: jeu.tonete
 *
 * Created on 2021.06
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PRODUCTCRTL_H
#define	PRODUCTCRTL_H

#include "StateMachine.h"

void productCtrl_RXHandler(char data);
void productCtrl_init(uint16_t machineStateHandle, xStateMachineEventFlags *flags);
void productCtrl_hardwareConfig(void);
void productCtrl_loop(void);


#endif	/* PRODUCTCRTL_H */

