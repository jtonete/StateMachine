#include "StateMachine.h"
#include <Arduino.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#ifndef STATEMACHINE_PORT_H
#define	STATEMACHINE_PORT_H

#if( STATEMACHINE_TICKHOOK > 0 )
extern void vStateMachineTickHook( void );
#endif

void vStateMachine_TickConfigure();

#define StateMachine_DisableTick noInterrupts();          // disable all interrupts
#define StateMachine_EnableTick  interrupts();             // enable all interrupts//
#define StateMachine_NOP         Serial.println("NOP")     //
#define StateMachine_LOG(log)    Serial.println(log)

#endif	/* STATEMACHINE_PORT_H */
