/***************************************************************************/ /**
 * 
 * Source File	: 	argoncustomfunctions.h
 * Author		: 	Daniel K. Vinther Wolf 
 * Created		:	20201024
 * Version		:	0.1.0
 * 
 * Description	:	LED Functions
 * 
 * 
 ******************************************************************************/

#include "Particle.h"
#include "pinsettings.h"

bool toggleBlueLED(bool BlueLED)
{
    if (!BlueLED)
    {
        digitalWrite(BLUE_LED, HIGH);
        return true;
    }
    else
    {
        digitalWrite(BLUE_LED, LOW);
        return false;
    }
}
