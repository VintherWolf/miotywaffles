/***************************************************************************/ /**
 * 
 * Source File	: 	touchsensor.h
 * Author		: 	Daniel K. Vinther Wolf 
 * Created		:	20201027
 * Version		:	0.1.0
 * 
 * Description	:	touchsensor
 *
 * 
 ******************************************************************************/
#include "Particle.h"
#include "../Settings/pinsettings.h"

#ifndef _TOUCHSENSOR_H_
#define _TOUCHSENSOR_H_

void initTouchSensor()
{
    pinMode(TOUCH_SENSOR, INPUT);
}

#endif