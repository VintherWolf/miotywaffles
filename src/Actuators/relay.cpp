/**
 * @file relay.cpp
 * @author Daniel K. Vinther Wolf
 * @brief Control Mains Relay (on/off)
 * @version 0.1
 * @date 2020-10-27
 * 
 * 
 */

#include "Particle.h"
#include "relay.h"

/**
 * @brief Initialize GPO for Relay
 * 
 */
void initRelay()
{
    pinMode(RELAY_WIRON, OUTPUT);
}

/**
 * @brief Set the Relay on/off
 * 
 * @param command 
 */
void setRelay(String command)
{
    command.toLowerCase();
    if (command.compareTo("on") == 0)
    {
        digitalWrite(RELAY_WIRON, HIGH);
    }
    if (command.compareTo("off") == 0)
    {
        digitalWrite(RELAY_WIRON, LOW);
    }
}