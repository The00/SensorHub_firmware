/*
 * timer4.cpp
 *
 *  Created on: 17 févr. 2017
 *      Author: theo
 */

#include "timer4.h"

void (*isrCallback)();

void setup_T4(unsigned int period, void (*isr)())
{
    TCCR4A = 0;
    TCCR4B = 0;
    TCCR4C = 0;
    TCCR4D = 0;
    TCCR4E = 0;

    setPeriod_T4(period);

    /// setup interrupt
    TIFR4 = 0;              // clear all interrupt flag
    isrCallback = isr;      // link the isr
    //TIMSK4 = _BV(TOIE4);    // enable overflow interrupt now in start_T4()
}

void setPeriod_T4(unsigned int period)
{
    TCCR4B &= 0xF0; // stop the timer


    /* compute reload and prescaler value */

    unsigned long target_prescaler = period*(F_CPU >> 10)/1000;

    for(unsigned int i =0;i<15;i++)
    {
        if((1<<i) > target_prescaler)
        {
            target_prescaler = (1<<i);
            break;
        }
    }


    unsigned long reload = F_CPU/(target_prescaler);
    reload *=period;
    reload /=1000;


    // Serial.print("prescaler: ");
    // Serial.println(target_prescaler);
    // Serial.print("reload: ");
    // Serial.println(reload);
    byte ps = (log(target_prescaler)/log(2))+1;
    // Serial.print("prescaler byte: ");
    // Serial.println(ps);


    /* write reload and prescaler value */

    TC4H = (reload >> 8);
    OCR4C = (byte)reload;      // reload value
    TCCR4B = ps;               // prescaler

/*
    Serial.print("prescaler register: ");
    Serial.println(TCCR4B);
    Serial.print("reload LSB: ");
    Serial.println(OCR4C);
    Serial.print("reload MSB: ");
    Serial.println(TC4H);
    */
}

void start_T4()
{
    /* reset the count register */
    TC4H = 0;
    TCNT4 = 0;

    /* enable interrupt */
    TIFR4 = 0;              // clear all interrupt flag
    TIMSK4 = _BV(TOIE4);    // enable overflow interrupt
}

void stop_T4()
{
    TIMSK4 &= ~(_BV(TOIE4));    // disable overflow interrupt
}


ISR(TIMER4_OVF_vect)
{
    isrCallback();
}


