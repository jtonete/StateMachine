/*
 * File:   productCtrl.c
 * Author: jeu.tonete
 *
 * Created on 2021.06
 */

#include "productCtrl.h"
#include "remoteAccessCore.h"

uint16_t productCtrl_MShandle __attribute__((packed));
xStateMachineEventFlags *productCtrl_MSeventflags __attribute__((packed));

void productCtrl_init(uint16_t machineStateHandle, xStateMachineEventFlags *flags)
{

    productCtrl_MShandle = machineStateHandle;
    productCtrl_MSeventflags = flags;

    vSM_setNextStateIdx(productCtrl_MShandle, productCtrl_hardwareConfig, 500);
}

void productCtrl_hardwareConfig(void)
{
    // configure GPIOs

    pinMode(13, OUTPUT);    // sets the digital pin 13 as output

    vSM_setNextStateIdx(productCtrl_MShandle, productCtrl_loop, 500);
}

void productCtrl_loop(void)
{
    switch (productCtrl_MSeventflags->flags)
    {
    case SET_1:
        digitalWrite(13, HIGH); // sets the digital pin 13 on
        productCtrl_MSeventflags->flags = 0;
        break;
    
    case SET_2:
        digitalWrite(13, LOW); // sets the digital pin 13 on
        productCtrl_MSeventflags->flags = 0;
        break;
    
    default:
        break;
    }
    
    vSM_setTimerIdx(productCtrl_MShandle, 10);
}