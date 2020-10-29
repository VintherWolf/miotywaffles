/**
 * @file anglesensor.h
 * @author Daniel K. Vinther Wolf
 * @brief Angle Sensor
 * @version 0.1
 * @date 2020-10-23
 * 
 * 
 */

#include "Particle.h"
#include "../Settings/pinsettings.h"

#ifndef _ANGLESENSOR_H_
#define _ANGLESENSOR_H_

void initAngleSensor();
bool lidIsOpen();

#endif