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
#include "sonos.hpp"

SonosControl::SonosControl()
{
}

SonosControl::~SonosControl()
{
    //
}

// Particle Cloud
void ParticleCloud::SonosPlay(std::string state)
{
    std::cout << state << std::endl;
}

bool ParticleCloud::ConnectedToCloud()
{
    return true;
}

void ParticleCloud::ConnectToCloud()
{
    //
}

void ParticleCloud::ShowPossibleStates()
{

    for (int i = 0; i < 3; i++)
    {
        std::cout << _states[i] << std::endl;
    }
}