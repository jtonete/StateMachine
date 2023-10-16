#include "mainApp.h"

/* State machine variable */
uint16_t mApp_MShandle;
xStateMachineEventFlags *mApp_MSeventflags;					// not required



void mainApp_setup(uint16_t machineStateHandle, xStateMachineEventFlags *flags)
{
	mApp_MShandle = machineStateHandle;						// internal handler receive the machine state addr
    mApp_MSeventflags = flags;

	/* Setup */
  
	Serial.println("mainApp setup");

	/* Set next state */
	vSM_setNextStateIdx(mApp_MShandle, mainApp_loop, 1);

}

void mainApp_loop()
{
	Serial.println("In mainApp loop function");
	vSM_setTimerIdx(mApp_MShandle, 500);
}


