/*
 * timer4.h
 *
 *  Created on: 17 févr. 2017
 *      Author: theo
 */

#ifndef LIB_TIMER4_SRC_TIMER4_H_
#define LIB_TIMER4_SRC_TIMER4_H_


#include <avr/io.h>
#include <avr/interrupt.h>
#include "Arduino.h"

#ifdef __AVR_ATmega32U4__
#endif

void setup_T4(unsigned int period, void (*isr)());
void setPeriod_T4(unsigned int period);
void stop_T4();
void start_T4();


#endif /* LIB_TIMER4_SRC_TIMER4_H_ */
