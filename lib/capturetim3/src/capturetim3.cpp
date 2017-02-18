/*
 * capturetim3.cpp
 *
 *  Created on: 17 févr. 2017
 *      Author: theo
 */


#include "capturetim3.h"

unsigned int captureVal =0;
void (*isrCapt)();

void initCaptureTim3(void (*isr)(), char edge, int prescaler)
{
  // Input Capture setup
  // ICNC3: Enable Input Capture Noise Canceler
  // ICES3: =0 for trigger on falling edge
  // CS30 and CS31: =1 set prescaler to 1/64 system clock (F_CPU)
  TCCR3A = 0;
  TCCR3B = 0;
  TCCR3C = 0;

  if(edge == RISING) TCCR3B |= (1<<ICES3);  // use arduino define FALLING = 2 and RISING =3
  TCCR3B |= (1<<ICNC3);                     // enable noise canceler
  setPrescaler(prescaler);

  // Interrupt setup
  // ICIE3: Input capture
  // TOIE3: Timer3 overflow : not used
  TIFR3 = (1<<ICF3) | (1<<TOV3);        // clear pending
  isrCapt = isr;                    // link user isr
  TIMSK3 = (1<<ICIE3);                  // and enable

  // Set up the Input Capture pin, ICP3, which corresponds to Arduino 13
  pinMode(ICP3_PIN, INPUT);
  digitalWrite(ICP3_PIN, 0);       // leave floating to count 60 Hz etc.

}


void setPrescaler(int prescaler)
{
    switch (prescaler)
    {
    case 1:
        TCCR3B |= (1<<CS30);
        break;
    case 8:
        TCCR3B |= (1<<CS31);
        break;
    case 64:
        TCCR3B |= (1<<CS30) |(1<<CS31);
        break;
    case 256:
        TCCR3B |= (1<<CS32);
        break;
    case 1024:
        TCCR3B |= (1<<CS30) | (1<<CS32);
        break;
    default:
        TCCR3B &= ~((1<<CS30) | (1<<CS31) | (1<<CS32)) ;

    }

}


void startCapture()
{
    TIFR3 = (1<<ICF3) | (1<<TOV3);        // clear pending
    TIMSK3 |= (1<<ICIE3);
}

void stopCapture()
{
    TIMSK3 &= ~(1<<ICIE3);
}


ISR(TIMER3_CAPT_vect) {

  captureVal = 0;
  captureVal = ICR3L;               // grab captured timer value
  captureVal |= (ICR3H<<8);          // grab captured timer value

  isrCapt();
}

