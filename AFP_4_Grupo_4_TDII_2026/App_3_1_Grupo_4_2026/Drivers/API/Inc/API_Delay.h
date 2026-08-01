/*
 * API_Delay.h
 *
 *  Created on: 25 Jul 2026
 *      Author: Gabriel Brancato
 */

#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_

/* includes */
#include <stdint.h>   // para uint32_t
#include <stdbool.h>  // para bool

/*exported types*/
typedef uint32_t tick_t;
typedef bool bool_t;
typedef struct{
	tick_t startTime;
	tick_t duration;
	bool_t running;
}delay_t;

void delayInit(delay_t * delay, tick_t duration);
bool_t delayRead(delay_t * delay);
void delayWrite(delay_t * delay, tick_t duration);

#endif /* API_INC_API_DELAY_H_ */
