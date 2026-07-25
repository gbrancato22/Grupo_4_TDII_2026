/*
 * API_GPIO.h
 *
 *  Created on: 25 jul 2026
 *      Author: Gabriel Brancato
 */

#ifndef API_INC_API_GPIO_H_
#define API_INC_API_GPIO_H_

/* includes */
#include <stdint.h> /* para poder usar uint16_t*/

/*exported types*/
typedef uint16_t led_t; /* debe ser uint16_t*/

void MX_GPIO_Init(void);
void writeLedOn_GPIO(led_t LDx);
void writeLedOff_GPIO(led_t LDx);
void toggleLed_GPIO(led_t LDx);

#endif /* API_INC_API_GPIO_H_ */
