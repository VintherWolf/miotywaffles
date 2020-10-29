/***************************************************************************/ /**
 * 
 * Source File	: 	relay.c
 * Author		: 	Daniel K. Vinther Wolf 
 * Created		:	20201027
 * Version		:	0.1.0
 * 
 * Description	:	Relay
 *
 * 
 ******************************************************************************/
#include "Particle.h"

#include "relay.h"

void initRelay()
{
    pinMode(RELAY_WIRON, OUTPUT);
}

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