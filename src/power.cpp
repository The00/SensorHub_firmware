/*
 * power.cpp
 *
 *  Created on: 17 févr. 2017
 *      Author: theo
 */

#include "Arduino.h"
#include "banc.h"


power_data read_power()
{
    power_data result;
    result.current=analogRead(P_CURRENT);
    result.voltage=analogRead(P_VOLTAGE);

    return result;
}


void resetPowerStruct(power_data * data)
{
    data->voltage = 0;
    data->current = 0;
}


power_data operator+(const power_data &a, const power_data &b)
    {
        power_data result;
        result.current = a.current + b.current;
        result.voltage = a.voltage + b.voltage;

        return result;
    }


power_data operator >> (const power_data &a, const int &shift)
{
    power_data result;
    result.voltage = a.voltage >> shift;
    result.current = a.current >> shift;

    return result;
}


