/**
 * @file relay.h
 * @author Daniel K. Vinther Wolf
 * @brief Relay
 * @version 0.1
 * @date 2020-10-27
 * 
 * 
 */
#include "Particle.h"
#include "../Settings/pinsettings.h"

#ifndef _RELAY_H_
#define _RELAY_H_

void initRelay();
void setRelay(String command);

#endif