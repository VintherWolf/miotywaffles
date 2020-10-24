/***************************************************************************/ /**
 * 
 * Source File	: 	anglesensor.hpp
 * Author		: 	Daniel K. Vinther Wolf 
 * Created		:	20201023
 * Version		:	0.1.0
 * 
 * Description	:	Angle Sensor Class
 *
 * 
 ******************************************************************************/
#include "Particle.h"
#include "../Settings/pinsettings.h"

/**
 * @brief Lid is OPENED when input (D2) is LOW (Refer to Electrical Diagram)
 * 
 * @return true 
 * @return false 
 */

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