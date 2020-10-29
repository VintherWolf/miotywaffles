/***************************************************************************/ /**
 * 
 * Source File	: 	anglesensor.h
 * Author		: 	Daniel K. Vinther Wolf 
 * Created		:	20201023
 * Version		:	0.1.0
 * 
 * Description	:	Angle Sensor
 *
 * 
 ******************************************************************************/
#include "Particle.h"
#include "../Settings/pinsettings.h"

#ifndef _ANGLESENSOR_H_
#define _ANGLESENSOR_H_

/**
 * @brief Lid is OPENED when input (D2) is LOW (Refer to Electrical Diagram)
 * 
 * @return true 
 * @return false 
 */

void initAngleSensor()
{
    pinMode(ANGLE_SENSOR, INPUT);
}

bool lidIsOpen()
{
    if (digitalRead(ANGLE_SENSOR) == LOW)
    {
        int counter = 0;
        while (digitalRead(ANGLE_SENSOR) == LOW)
        {
            // Pin must be LOW for some time to be valid LOW
            counter++;
            if (counter >= 2000000)
            {
                return true;
            }
        }
    }
    else
    {
        return false;
    }
}

#endif