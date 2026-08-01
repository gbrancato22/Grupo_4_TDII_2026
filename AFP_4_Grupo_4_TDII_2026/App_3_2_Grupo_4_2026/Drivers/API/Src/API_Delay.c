/*
 * API_Delay.c
 *
 *  Created on: 25 Jul 2026
 *      Author: Gabriel Brancato
 */

/* includes */
#include "main.h"
#include "API_Delay.h"

// Declaration of variables

// function definition

void delayInit(delay_t * delay, tick_t duration)
{
    delay->duration = duration;
    delay->running = false;
}

bool_t delayRead(delay_t * delay)
{
    bool_t ret = false;

    if(delay->running == false)
    {
        delay->startTime = HAL_GetTick();
        delay->running = true;
    }
    else
    {
        if((HAL_GetTick() - delay->startTime) >= delay->duration)
        {
            delay->running = false;
            ret = true;
        }
    }

    return ret;
}

void delayWrite(delay_t * delay, tick_t duration)
{
    delay->duration = duration;
}



