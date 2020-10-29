/**
 * @file anglesensor.cpp
 * @author Daniel K. Vinther Wolf
 * @brief Check if lid is open or closed
 * @version 0.1
 * @date 2020-10-23
 * 
 * 
 */

#include "Particle.h"
#include "anglesensor.h"

/**
 * @brief init GPI for Anglesensor
 * 
 */
void initAngleSensor()
{
    pinMode(ANGLE_SENSOR, INPUT);
}

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
