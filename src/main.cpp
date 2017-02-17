/*
 Arduino Code v3.0
*/
//#include <digitalFast.h>
#include <timer4.h>
#include <banc.h>
#include <TimerOne.h>


/// Variables globales /

rpm_data rpm;
thrust_data thrust;
power_data power;

unsigned int nbr_sample = MAX_RPM_SAMPLE;   // not used in this version, kept for comptability
unsigned int timeout =40;                  // not used in this version, kept for comptability
unsigned int sampling_delay = 100;

byte state_s;
mode_enum mode= CONFIGURATION;


power_data power_sample[NBR_ANALOG_SAMPLE];
thrust_data thrust_sample[NBR_ANALOG_SAMPLE];

char trame[50];
char buffer_in[10];
char buffer_out[4];
volatile char analog_count = 0;
volatile char flag =0;
int byte_avail=0;

unsigned int esc_value = 900;


/* sampling rate matching */
int lasttempo =0;
int tempo =0;
int diff=0;



void callback_isr()
{
    power_sample[analog_count]   = read_power();
    thrust_sample[analog_count]  = read_thrust();
    analog_count ++;

    /* when the sampling is over */
    if(analog_count == NBR_ANALOG_SAMPLE)
    {
        analog_count = 0;
        /* disable interrupt */
        stop_T4();
        stopCapture();
        flag = 1;
    }

}

void setup() {
 Serial.begin(115200);
 init_pins();
 init_rpm();

 setup_T4(sampling_delay >> ANALOG_SHIFT, callback_isr);

 Timer1.initialize(20000); // timer1 generates the PWM for the ESC (period = 20ms)
 Timer1.pwm(ESC_OUT, 50); // 50 (on 1024) = duty cycle ~ 950µs

 start_T4();
}

void loop() {

  if(mode==CONFIGURATION)
  {
       byte_avail =Serial.available();
       if(byte_avail)
       {
         Serial.readBytes(buffer_in,byte_avail);
         decode_trame(buffer_in,&mode,&sampling_delay,&nbr_sample,&timeout);
         setPeriod_T4(sampling_delay >> ANALOG_SHIFT);
       }

     getState_sensors(&state_s);
     sprintf(buffer_out,"%03d",state_s);
     Serial.println(buffer_out);
     delay(50);
  }

   else if(mode==ACQUISITION)
  {

    byte_avail =Serial.available();
    if(byte_avail)
    {
      Serial.readBytes(buffer_in,byte_avail);
      esc_value = atoi(buffer_in);
      if(esc_value == 0)
       {
         Timer1.setPwmDuty_micros(ESC_OUT, ESC_LOW);
         mode = CONFIGURATION;
       }
       else Timer1.setPwmDuty_micros(ESC_OUT, esc_value); //ESC
     }


     /* if acquisition is done, time to process and send data  */
     if(flag)
     {

       flag =0;

       /* process analog sensor data */
       resetPowerStruct(&power);
       resetThrustStruct(&thrust);
       /* average */
       for(byte_avail =0; byte_avail<NBR_ANALOG_SAMPLE;byte_avail++)
       {
          thrust = thrust + thrust_sample[byte_avail];
          power = power + power_sample[byte_avail];
       }

       thrust = thrust >> ANALOG_SHIFT;
       power  = power  >> ANALOG_SHIFT;


       /* process RPM data  */
        getRpmData(&rpm);
        build_data_string(trame,&rpm,&thrust,&power,&state_s);

        /* little delay to match with the target sampling delay */
        tempo = millis();
        diff = (int)(sampling_delay-(tempo-lasttempo));
        if(diff>0) delay(diff);

        /* Send data*/
        Serial.print(trame);
        lasttempo = tempo;

        /* re-enable interrupt */
        start_T4();
        startCapture();
      }

  }

}