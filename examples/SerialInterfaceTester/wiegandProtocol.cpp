/*
 * File:   wiegandProtocol.c
 * Author: jeu.tonete
 *
 * Created on 2021.06
 */

#include "wiegandProtocol.h"

uint16_t wiegandProtocol_MShandle __attribute__((packed));
xStateMachineEventFlags *wiegandProtocol_MSeventflags __attribute__((packed));
Wiegand wiegand __attribute__((packed));

void wiegandProtocol_init(uint16_t machineStateHandle, xStateMachineEventFlags *flags)
{

    wiegandProtocol_MShandle = machineStateHandle;
    wiegandProtocol_MSeventflags = flags;

    vSM_setNextStateIdx(wiegandProtocol_MShandle, wiegandProtocol_hardwareConfig, 500);
}

void wiegandProtocol_hardwareConfig(void)
{
    // configure GPIOs

    pinMode(13, OUTPUT);    // sets the digital pin 13 as output

    wiegand.SendData = false;
    wiegand.ReadyToSend = true;

    vSM_setNextStateIdx(wiegandProtocol_MShandle, wiegandProtocol_loop, 500);
}

void wiegandProtocol_loop(void)
{
    if(wiegand.SendData)
    {
        wiegand.SendData = false;
        switch (wiegand.dataSizeInBytes)
        {
        case 1:
            
            break;
        case 2:
        
            break;
        case 3:
        
            break;
        
        default:
            break;
        }

        wiegand.ReadyToSend = true;
    }

    vSM_setTimerIdx(wiegandProtocol_MShandle, 10);

    
}

void wiegandProtocol_SetDataToSend(uint8_t data[8], uint8_t size)
{
    wiegand.SendData = true;
    wiegand.ReadyToSend = false;
    memcpy(wiegand.data, data, size);
}

bool wiegandProtocol_IsReadyToSend(void)
{
    return wiegand.ReadyToSend;
}

// outputs a 34 bit Wiegand code
// u32 is actually the 24-bit numeric code
void outwieg34(uint32_t u32)
{
  uint32_t tmp = u32;
  unsigned int p_even = 0;
  unsigned int p_odd = 1;
  // compute parity on trailing group of bits 
  for (int n=0; n<16; ++n)
  {
    p_odd ^= (tmp & 1);
    tmp >>= 1;
  }
  // compute parity on heading group of bits
  for (int n=16; n<32; ++n)
  {
    p_even ^= (tmp & 1);
    tmp >>= 1;
  }
  // now output data bits framed by parity ones
  outwiegbit(p_even);
  for (int n=0; n<32; ++n)
  {
    outwiegbit((u32 >> (31-n)) & 1);
  }
  outwiegbit(p_odd);  
}

// outputs a 26 bit Wiegand code
// u32 is actually the 24-bit numeric code
void outwieg26(uint32_t u32)
{
  uint32_t tmp = u32;
  unsigned int p_even = 0;
  unsigned int p_odd = 1;
  // compute parity on trailing group of bits 
  for (int n=0; n<12; ++n)
  {
    p_odd ^= (tmp & 1);
    tmp >>= 1;
  }
  // compute parity on heading group of bits
  for (int n=12; n<24; ++n)
  {
    p_even ^= (tmp & 1);
    tmp >>= 1;
  }
  // now output data bits framed by parity ones
  outwiegbit(p_even);
  for (int n=0; n<24; ++n)
  {
    outwiegbit((u32 >> (23-n)) & 1);
  }
  outwiegbit(p_odd);  
}