/***************************************************************************/ /**
 * 
 * Source File	: 	relay.h
 * Author		: 	Daniel K. Vinther Wolf 
 * Created		:	20201027
 * Version		:	0.1.0
 * 
 * Description	:	Relay
 *
 * 
 ******************************************************************************/
#include "Particle.h"
#include "../Settings/pinsettings.h"

#ifndef _RELAY_H_
#define _RELAY_H_

// initialize GPO pin for relay
void initRelay();

// Set relay "on" or "off"
void setRelay(String command);

#endif