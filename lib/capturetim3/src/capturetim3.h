/*
 * capturetim3.h
 *
 *  Created on: 17 févr. 2017
 *      Author: theo
 */

#ifndef LIB_CAPTURETIM3_SRC_CAPTURETIM3_H_
#define LIB_CAPTURETIM3_SRC_CAPTURETIM3_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Arduino.h"

#ifdef __AVR_ATmega32U4__
#define ICP3_PIN 13
#endif


extern unsigned int captureVal;



void setPrescaler(int prescaler);
void initCaptureTim3(void (*isr)(), char edge, int prescaler);
void startCapture();
void stopCapture();




#endif /* LIB_CAPTURETIM3_SRC_CAPTURETIM3_H_ */
