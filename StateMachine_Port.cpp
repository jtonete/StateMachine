#include "StateMachine_Port.h"

#if( STATEMACHINE_TICKHOOK > 0 )
void vStateMachineTickHook( void )
{
    #warning "implements vStateMachineTickHook()"
}
#endif

void set_frequency_timer1(unsigned long frequency);
void init_timer1();
bool toogle = true;

ISR(TIMER1_COMPA_vect)
{
  vSM_Tick();
}

void vStateMachine_TickConfigure()
{
	/* configure here a timer with 1ms that must call vSM_Tick() function in ISR */
	StateMachine_LOG("Tick Configure...");
   
	StateMachine_DisableTick;
	init_timer1();
	set_frequency_timer1(500);
	
	StateMachine_EnableTick;
}

void set_frequency_timer1(unsigned long frequency)
{
  uint8_t prescalarbits = 0b001; // Preseted as 1

  uint64_t ocr = F_CPU / frequency / 2 - 1; // timeout counter

  if (ocr > 0xffff)
  {
  ocr = F_CPU / frequency / 2 / 64 - 1;
    prescalarbits = 0b011;
  }

   TCCR1B = (TCCR1B & 0b11111000) | prescalarbits;

   OCR1A = ocr; // LOAD comparison register
   TIMSK1 |= (1 << OCIE1A); // Enable Timer/Counter1 compare match B

}

void init_timer1()
{
  TCCR1A = 0;         // Configure timer to normal operation OC1A and OC1B disconected
  TCCR1B = 0;         // Clear register
  TCCR1B |= (1 << WGM12);   // CTC Mode
  TCCR1B |= (1 << CS10);    // Define prescaler to 1
}



