/*
 * File:   StateMachine.c
 * Author: jeu.tonete
 *
 * Created on 19 de Mar�o de 2019, 17:39
 */

#include "StateMachine.h"

/*
 * Handlers Storage
 *      counter / size / pointer
 */
xStateMachineHandler xSMacHandler;

#if( STATEMACHINE_MILLIS > 0 )
uint32_t uSMmillis;
#endif

/*
 * Function: vSM_Tick
 *
 *
*/
void vSM_Tick(void)
{

    int i;
    for(i = 0; i<xSMacHandler.handlerSize; i++)
    {
        if(xSMacHandler.handlerStorage[i].stateTimer.enable)         /* Ublox State Machine timer, time between states  */
        {
            if((--xSMacHandler.handlerStorage[i].stateTimer.currentTime)<=0)
            {
                xSMacHandler.handlerStorage[i].stateTimer.enable = 0;
                xSMacHandler.handlerStorage[i].stateTimer.timeout = 1;
            }
        }
        if(xSMacHandler.handlerStorage[i].stateTimeoutTimer.enable)  /* Ublox State Machine timer, state timeout  */
        {
            if((--xSMacHandler.handlerStorage[i].stateTimeoutTimer.currentTime)<=0)
            {
                xSMacHandler.handlerStorage[i].stateTimeoutTimer.enable = 0;
                xSMacHandler.handlerStorage[i].stateTimeoutTimer.timeout = 1;
            }
        }
    }

#if (STATEMACHINE_TICKHOOK > 0)
    vStateMachineTickHook();
#endif
#if( STATEMACHINE_MILLIS > 0 )
	uSMmillis++;
#endif

}

#if( STATEMACHINE_MILLIS > 0 )
uint32_t vStateMachineMillis( void )
{
	StateMachine_DisableTick;
	uint32_t temp = uSMmillis;
	StateMachine_EnableTick;
	return temp;

}
#endif

/*
 * Function: vSM_Init
 *
 * initializes the timer 5 for 1ms interrupts
*/

void vSM_Init(xStateMachine *pMachines, uint16_t storageSize)
{
    int i;
    vStateMachine_TickConfigure(); // configure 1ms timer to StateMachine engine

    xSMacHandler.handlerIdx = 0;
    xSMacHandler.handlerSize = storageSize;
    xSMacHandler.handlerStorage = pMachines;

    memset((void*)xSMacHandler.handlerStorage, 0, storageSize * sizeof(xStateMachine));

    for(i = 0; i<xSMacHandler.handlerSize; i++)
    {
        vSM_setNextState(&xSMacHandler.handlerStorage[i], vSM_nop, 1);
    }

#if( STATEMACHINE_MILLIS > 0 )
	uSMmillis=0;
#endif

}

void vSM_RunAtInfinitLoop()
{
    int i;

    for(i = 0; i<xSMacHandler.handlerSize; i++)
    {
        if(xSMacHandler.handlerStorage[i].stateTimer.timeout)
        {
            xSMacHandler.handlerStorage[i].state();
        }
    }
}

void vSM_setTimer(xStateMachine *self, uint32_t time)
{
    StateMachine_DisableTick;
    self->stateTimer.enable = true;
    self->stateTimer.timeout = false;
    self->stateTimer.currentTime = time;
    StateMachine_EnableTick;
}

void vSM_setTimerIdx(uint16_t idx, uint32_t time)
{
    if(idx < xSMacHandler.handlerSize)
    {
        StateMachine_DisableTick;
        xSMacHandler.handlerStorage[idx].stateTimer.enable = true;
        xSMacHandler.handlerStorage[idx].stateTimer.timeout = false;
        xSMacHandler.handlerStorage[idx].stateTimer.currentTime = time;
        StateMachine_EnableTick;
    }
}

void vSM_setTimerIdxAndType(uint16_t idx, uint32_t time, uint16_t type)
{
    if(idx < xSMacHandler.handlerSize)
    {
        StateMachine_DisableTick;
        xSMacHandler.handlerStorage[idx].stateTimer.enable = true;
        xSMacHandler.handlerStorage[idx].stateTimer.timeout = false;
        xSMacHandler.handlerStorage[idx].stateTimer.currentTime = time;
        xSMacHandler.handlerStorage[idx].stateType = type;
        StateMachine_EnableTick;
    }
}

void vSM_setNextState(xStateMachine *self, fpStateMachine pvFunction, uint32_t time)
{
    StateMachine_DisableTick;
    self->state = pvFunction;
    self->stateTimer.enable = true;
    self->stateTimer.timeout = false;
    self->stateTimer.currentTime = time;
    StateMachine_EnableTick;
}

void vSM_setNextStateIdx(uint16_t idx, fpStateMachine pvFunction, uint32_t time)
{
    if(idx < xSMacHandler.handlerSize)
    {
        StateMachine_DisableTick;
        xSMacHandler.handlerStorage[idx].state = pvFunction;
        xSMacHandler.handlerStorage[idx].stateTimer.enable = true;
        xSMacHandler.handlerStorage[idx].stateTimer.timeout = false;
        xSMacHandler.handlerStorage[idx].stateTimer.currentTime = time;
        StateMachine_EnableTick;
    }
}

void vSM_setNextStateIdxAndType(uint16_t idx, fpStateMachine pvFunction, uint32_t time, uint16_t type)
{
    if(idx < xSMacHandler.handlerSize)
    {
        StateMachine_DisableTick;
        xSMacHandler.handlerStorage[idx].state = pvFunction;
        xSMacHandler.handlerStorage[idx].stateTimer.enable = true;
        xSMacHandler.handlerStorage[idx].stateTimer.timeout = false;
        xSMacHandler.handlerStorage[idx].stateTimer.currentTime = time;
        xSMacHandler.handlerStorage[idx].stateType = type;
        StateMachine_EnableTick;
    }
}

void vSM_setTimeoutTimer(xStateMachine *self, uint32_t time)
{
    StateMachine_DisableTick;
    self->stateTimeoutTimer.enable = true;
    self->stateTimeoutTimer.timeout = false;
    self->stateTimeoutTimer.currentTime = time;
    StateMachine_EnableTick;
}

void vSM_setTimeoutTimerIdx(uint16_t idx, uint32_t time)
{
    if(idx < xSMacHandler.handlerSize)
    {
        StateMachine_DisableTick;
        xSMacHandler.handlerStorage[idx].stateTimeoutTimer.enable = true;
        xSMacHandler.handlerStorage[idx].stateTimeoutTimer.timeout = false;
        xSMacHandler.handlerStorage[idx].stateTimeoutTimer.currentTime = time;
        StateMachine_EnableTick;
    }
}

uint16_t vSM_timeoutIdx(uint16_t idx)
{
    return xSMacHandler.handlerStorage[idx].stateTimeoutTimer.timeout;
}

uint8_t vSM_stateTypeIdx(uint16_t idx)
{
    return xSMacHandler.handlerStorage[idx].stateType;
}

void vSM_idle(xStateMachine *self)
{
    vSM_setTimer(self, 250);
}

void vSM_nop()
{
    StateMachine_NOP;
}

xStateMachineEventFlags vSM_initEventFlags()
{
    xStateMachineEventFlags foo;
    foo.flags=0;
    return foo;
}

void vSM_setEventFlags(xStateMachineEventFlags *flag, uint16_t uBitsToSet)
{
    flag->flags |= uBitsToSet;
}

void vSM_clearEventFlags(xStateMachineEventFlags *flag, uint16_t uBitsToClear)
{
    flag->flags &= ~uBitsToClear;
}

bool vSM_testEventFlags(xStateMachineEventFlags *flag, uint16_t uBitsToTest)
{
    return ( (flag->flags & uBitsToTest) ? true:false);
}

