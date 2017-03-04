/*
 * banch.h
 *
 *  Created on: 17 févr. 2017
 *      Author: theo
 */

#ifndef SRC_BANC_H_
#define SRC_BANC_H_

#include "Arduino.h"
#include "capturetim3.h"

/// --- MAPPING ARDUINO --- ///

#define HC05_TX     0
#define HC05_RX     1
#define HC05_COM    2
#define HC05_API    3
#define HC05_PWR    7



#define THRUST_IN1  A1
#define THRUST_IN2  A0
#define P_CURRENT   A3
#define P_VOLTAGE   A2
#define CTN_PLUS 	A4
#define CTN_MINUS 	A5

#define RPM_INPUT ICP3_PIN  // no choice : ICP3 (13 on leonardo)
#define ESC_OUT 11

#define P_PLUGGED   4
#define T1_PLUGGED  5
#define T1_ENABLED  6
#define T2_PLUGGED  8
#define T2_ENABLED  9
#define RPM_PLUGGED 12
#define RPM_ENABLED 10




#define RPM_PRESCALER       64
#define FREQ_TIMER          ((unsigned long)(F_CPU/RPM_PRESCALER))
#define NBR_ANALOG_SAMPLE   8
#define ANALOG_SHIFT       ((unsigned int)(log(NBR_ANALOG_SAMPLE)/log(2)))
#define MAX_ECARTYPE        20
#define MAX_RPM_SAMPLE      100
#define MAX_TIMEOUT         100
//#define COEFF1              1.03
//#define COEFF2              1.05
//#define B1_T -0.63
//#define B2_T -0.71
#define DEL_STR             ";"
#define DEL_CAR             ';'
#define L_TRAME_CONFIG      20
#define ESC_LOW             900

/// --- RPM --- ///

struct rpm_data{

    float rpm; // vitesse mesurée
    float ecart_type; // permet de voir si la mesure est valable
    int time;  // temps d'execution de la mesure en ms
    int nbr_sample_r;

} ;
typedef struct rpm_data rpm_data;

void rpm_isr();
void init_rpm();
void getRpmData(rpm_data *result);
float calcul_ecart_type(int tailletab, unsigned int data[],float moyenne);
float calcul_moyenne(int tailletab, unsigned int data[]);


/// --- THRUST --- ///

struct thrust_data{

   unsigned int sensor1;
   unsigned int sensor2;

} ;
typedef struct thrust_data thrust_data;

thrust_data operator +(const thrust_data &a, const thrust_data &b);
thrust_data operator >> (const thrust_data &a, const int &shift);
thrust_data read_thrust();
void resetThrustStruct(thrust_data *data);

/// --- POWER --- ///

struct power_data{
   unsigned int voltage;
   unsigned int current;
};

typedef struct power_data power_data;


power_data operator +(const power_data &a, const power_data &b);
power_data operator >> (const power_data &a, const int &shift);
power_data read_power();
void resetPowerStruct(power_data *data);

/// --- STATE SENSORS ---///

void init_pins();
void getState_sensors(byte *state);


/// --- DATA --- ///

enum serial_port {
	NONE,
	USB,
	HC05
};

enum mode_enum {
CONFIGURATION,
ACQUISITION
};

//void goto_acq_mode(mode_enum *mode);
//void goto_conf_mode(mode_enum *mode);

int ftoa(char *a, double f, int precision);
int build_data_string(char*str,rpm_data *rpm, thrust_data *thrust, power_data *power,byte *state);
void decode_trame(char buffer[],mode_enum *mode,unsigned int *sampling_delay, unsigned int *nbr_sample, unsigned int *timeout); // decode la trame entrante et affecte les variables avec les nouvelles valeurs


#endif /* SRC_BANC_H_ */
