/**
 * @file touchsensor.cpp
 * @author Daniel K. Vinther Wolf
 * @brief User input to start process
 * @version 0.1
 * @date 2020-10-27
 * 
 * 
 */

#include "Particle.h"
#include "touchsensor.h"

void initTouchSensor()
{
    pinMode(TOUCH_SENSOR, INPUT);
}
