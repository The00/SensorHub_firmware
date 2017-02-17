/*
 * rpm.cpp
 *
 *  Created on: 17 févr. 2017
 *      Author: theo
 */


#include "Arduino.h"
#include "banc.h"
#include "capturetim3.h"

/// Global variables ///
unsigned int lastVal;
char rpm_count;
unsigned int rpm_sample[MAX_RPM_SAMPLE];

void rpm_isr()
{
    /* captureVal defined in capturetim3 driver */
   rpm_sample[rpm_count] = (captureVal-lastVal);
   lastVal = captureVal;
   rpm_count++;
   if(rpm_count == MAX_RPM_SAMPLE) rpm_count =0;

}

void init_rpm()
{
    rpm_count = 0;
    lastVal =0;
    initCaptureTim3(rpm_isr, RISING, RPM_PRESCALER);
}

float calcul_moyenne(int tailletab, unsigned int data[])
{   byte i_local;
    unsigned long result=0;

    for(i_local=0;i_local<tailletab;i_local++) result +=data[i_local];

    return (float(result)/tailletab);
}

float calcul_ecart_type(int tailletab, unsigned int data[],float moyenne)
{
    byte i_local;
    float result=0;

   /* Serial.println();
    Serial.println("debug ecartype");
    Serial.print("taille tab: ");
    Serial.println(tailletab);
    Serial.print("moyenne: ");
    Serial.println(moyenne);
*/
    for(i_local=0;i_local<tailletab;i_local++) result +=sq(data[i_local]-moyenne);
/*
    Serial.print("somme: ");
    Serial.println(result);
    Serial.println(result/tailletab);
*/
    return sqrt(result/tailletab);
}

void switch_extremum(int tailletab, unsigned int data[],float moyenne)
{
    byte i_local=0;
    byte i_extr=0;
    float transit=0;
    float transit2=0;

    transit=abs(data[i_local]-moyenne);
    for(i_local=1;i_local<tailletab;i_local++)
    {
       transit2=abs(data[i_local]-moyenne);
       if(transit2>transit)
       {
           i_extr=i_local;
           transit=transit2;
       }
    }

    data[i_extr]=data[tailletab-1];

}



void analyse_data(int tailletab, unsigned int data[], rpm_data *result)
{
    static byte nb_appel =0;

    result->rpm = calcul_moyenne(tailletab,data);
    result->ecart_type = calcul_ecart_type(tailletab,data,result->rpm);
    //Serial.print("ecart type: ");
    //Serial.println(result->ecart_type);

    if((result->ecart_type) > MAX_ECARTYPE)
    {
        if(tailletab > ((tailletab+nb_appel)/2)) // si on a éliminé moins de la moitié des valeurs
        {   nb_appel++;
            switch_extremum(tailletab,data,result->rpm);
            analyse_data(tailletab-1,data,result);
        }
        else // la série de valeurs est inexploitable
        {
            result->rpm=-1;
            result->nbr_sample_r=tailletab;
        }
    }
    else result->nbr_sample_r=tailletab;

    nb_appel=0;
}


void getRpmData(rpm_data *result)
{
    analyse_data((rpm_count-1), &rpm_sample[1], result); // the first sample of rpm_sample is just the reference, not used to compute RPM

    if(result->rpm >0) result->rpm =  60*FREQ_TIMER/(result->rpm);
   // if(result->rpm >0) result->rpm =  60/(result->rpm*pow(10,-6));  // conversion period -> tr/mn

    result->time = -1; // in this driver version, conversion time is not relevant.

    /* reset these variable for the next measurement */
    rpm_count = 0;
    lastVal =0;
    startCapture();      // reset timer3
}




