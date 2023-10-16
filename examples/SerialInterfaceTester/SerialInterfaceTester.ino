#include <StateMachine.h>
#include <StateMachine_Port.h>

/* Include here the State Machine independet files */
#include "bluetoothControl.h"
#include "productCtrl.h"

/* State Machine */
enum
{
	/* Add here all state machine of project */
    BLUETOOTH_MACHINE = 0,
    PRODUCTCTRL,

	/* Last Item */
    STATEMACHINE_OBJECTS_QTY	// must not remove!!!
};

xStateMachineEventFlags eventsControl;
xStateMachine xStateMachinery[STATEMACHINE_OBJECTS_QTY];

void setup() {
  /* Init serial */
  Serial.begin(115200);
  //Serial.println("Init Setup");
  delay(10);

  vSM_Init(xStateMachinery, STATEMACHINE_OBJECTS_QTY);	// Initialize the StateMachine Core
  eventsControl = vSM_initEventFlags();					        // Initialize the events controller for communication between differents machine states

  bluetoothControl_init(BLUETOOTH_MACHINE, &eventsControl);
  productCtrl_init(PRODUCTCTRL, &eventsControl);

  /* Create here the new machines*/
}

void loop() {
  vSM_RunAtInfinitLoop();         // State machine run
}
