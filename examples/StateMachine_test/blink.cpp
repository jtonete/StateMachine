#include "blink.h"

/* State machine variable */
uint16_t blink_MShandle;
xStateMachineEventFlags *blink_MSeventflags;					// not required



void blink_setup(uint16_t machineStateHandle, xStateMachineEventFlags *flags)
{
	blink_MShandle = machineStateHandle;						// internal handler receive the machine state addr
  blink_MSeventflags = flags;

	/* Setup */
	Serial.println("blink setup");
  pinMode(2, OUTPUT);

	/* Set next state */
	vSM_setNextStateIdx(blink_MShandle, blink_st0, 1);

}

void blink_st0()
{
  Serial.println("In blink st0 function");
  digitalWrite(2, true);
 
  vSM_setNextStateIdx(blink_MShandle, blink_st1, 1000);
}

void blink_st1()
{
  Serial.println("In blink st1 function");
  digitalWrite(2, false);
 
	vSM_setNextStateIdx(blink_MShandle, blink_st0, 500);
}
