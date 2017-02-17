/*
 * thrust.cpp
 *
 *  Created on: 17 févr. 2017
 *      Author: theo
 */

#include "Arduino.h"
#include "banc.h"

thrust_data read_thrust()
{
    thrust_data result;
    result.sensor1=analogRead(THRUST_IN1);
    result.sensor2=analogRead(THRUST_IN2);

    return result;
}

void resetThrustStruct(thrust_data * data)
{
    data->sensor1 = 0;
    data->sensor2 = 0;
}

thrust_data operator+(const thrust_data &a, const thrust_data &b)
    {
        thrust_data result;
        result.sensor1 = a.sensor1 + b.sensor1;
        result.sensor2 = a.sensor2 + b.sensor2;

        return result;
    }

thrust_data operator >> (const thrust_data &a, const int &shift)
{
    thrust_data result;
    result.sensor1 = a.sensor1 >> shift;
    result.sensor2 = a.sensor2 >> shift;

    return result;
}




