/*
 * File:
 * Author:
 * Comments:
 * Revision history:
 */

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef STATE_MACHINE_H
#define	STATE_MACHINE_H

#include "StateMachine_Port.h"
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define STATEMACHINE_TICKHOOK	0
#define STATEMACHINE_MILLIS		1


#if( STATEMACHINE_TICKHOOK > 0 )
	extern void vStateMachineTickHook( void );
#endif

#if( STATEMACHINE_MILLIS > 0 )
	uint32_t vStateMachineMillis( void );
#endif

extern void vStateMachine_TickConfigure( void );

typedef void (*fpStateMachine)(void);

typedef struct _sTimerMachine{
	int32_t currentTime;
	uint16_t enable:1;
	uint16_t timeout:1;
	uint16_t :14;
} xTimeMachine;

typedef struct _sStateMachine {
	fpStateMachine state;
	xTimeMachine stateTimer;
	xTimeMachine stateTimeoutTimer;
	uint8_t stateType;
} xStateMachine;

typedef struct _sStateMachineHandler {
	uint16_t         handlerIdx;
	uint16_t         handlerSize;
	xStateMachine   *handlerStorage;
} xStateMachineHandler;

typedef struct _sStateMachineEventFlags {
	union{
		uint16_t flags;
		struct{
			uint16_t flag0:1;
			uint16_t flag1:1;
			uint16_t flag2:1;
			uint16_t flag3:1;
			uint16_t flag4:1;
			uint16_t flag5:1;
			uint16_t flag6:1;
			uint16_t flag7:1;
			uint16_t flag8:1;
			uint16_t flag9:1;
			uint16_t flag10:1;
			uint16_t flag11:1;
			uint16_t flag12:1;
			uint16_t flag13:1;
			uint16_t flag14:1;
			uint16_t flag15:1;
		};
	};
} xStateMachineEventFlags;


void vSM_Tick(void);
void vSM_Init(xStateMachine *pMachines, uint16_t storageSize);
void vSM_setTimer(xStateMachine *self, uint32_t time);
void vSM_setTimerIdx(uint16_t idx, uint32_t time);
void vSM_setTimerIdxAndType(uint16_t idx, uint32_t time, uint16_t type);
void vSM_setNextState(xStateMachine *self, fpStateMachine pvFunction, uint32_t time);
void vSM_setNextStateIdx(uint16_t idx, fpStateMachine pvFunction, uint32_t time);
void vSM_setNextStateIdxAndType(uint16_t idx, fpStateMachine pvFunction, uint32_t time, uint16_t type);
void vSM_setTimeoutTimer(xStateMachine *self, uint32_t time);
void vSM_setTimeoutTimerIdx(uint16_t idx, uint32_t time);
uint16_t vSM_timeoutIdx(uint16_t idx);
uint8_t vSM_stateTypeIdx(uint16_t idx);
void vSM_RunAtInfinitLoop();

void vSM_idle();
void vSM_nop();

xStateMachineEventFlags vSM_initEventFlags();
void vSM_setEventFlags(xStateMachineEventFlags *flag, uint16_t uBitsToSet);
void vSM_clearEventFlags(xStateMachineEventFlags *flag, uint16_t uBitsToClear);
bool vSM_testEventFlags(xStateMachineEventFlags *flag, uint16_t uBitsToTest);

#endif	/* STATE_MACHINE_H */

