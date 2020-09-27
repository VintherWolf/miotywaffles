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

/***********************************************************************
 *  
 ***********************************************************************/

#include "Particle.h"

#ifndef _SONOS_HPP_
#define _SONOS_HPP_

class SonosControl
{
public:
    /* Attributes */
    const String _event = "waffleiron_sonos";
    const String _states[3] = {"heating", "ready", "baking"};

    /* Methods */
    SonosControl();
    ~SonosControl();
    bool SonosPlay(String state);
    bool ConnectedToCloud(void);
    void ConnectToCloud(void);

private:
    /* Attributes */
    bool _connectedToCloud;
    bool _publishToCloudSuccess;
};

#endif
