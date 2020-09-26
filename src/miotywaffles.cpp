/***************************************************************************/ /**
 * 
 * Source File	: 	miotywaffles.cpp
 * Author		: 	Daniel K. Vinther Wolf 
 * Created		:	20200926
 * Version		:	0.1.0
 * 
 * Description	:	MiotyWaffles 
 * 
 * 
 ******************************************************************************/

#include <iostream>
#include <string>

#include "sonos.hpp"

int main(int argc, char *argv[])
{
    ParticleCloud sc;

    sc.SonosPlay("heating");
    sc.ShowPossibleStates();

    return 0;
}