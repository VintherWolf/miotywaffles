/***************************************************************************/ /**
 * 
 * Source File	: 	sonos.cpp
 * Author		: 	Daniel K. Vinther Wolf 
 * Created		:	20200923
 * Version		:	0.1.0
 * 
 * Description	:	Sonos API
 * 
 * 
 ******************************************************************************/

#include "Particle.h"
#include "sonos.hpp"

/***********************************************************************
 * 
 ***********************************************************************/

SonosControl::SonosControl()
{
    _connectedToCloud = false;
    _publishToCloudSuccess = false;
}

SonosControl::~SonosControl()
{
    //
}

bool SonosControl::SonosPlay(String state)
{
    _publishToCloudSuccess = Particle.publish(_event, state, PUBLIC);
    return _publishToCloudSuccess;
}

bool SonosControl::ConnectedToCloud()
{
    //
}

void SonosControl::ConnectToCloud()
{
    //
}