/*
 * API_Debounce.h
 *
 *  Created on: 4 oct 2026
 *      Author: Brancato Gabriel
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

#include "API_delay.h"

// Estados de la MEF
typedef enum {
	BUTTON_UP ,
	BUTTON_FALLING ,
	BUTTON_DOWN ,
	BUTTON_RISING
} debounceState_t;

// Funciones
void debounceFSM_init(void);
void debounceFSM_update(bool_t buttonRead);
bool_t readKey(void);
void buttonPressed(void);
void buttonReleased(void);

#endif /* API_INC_API_DEBOUNCE_H_ */
