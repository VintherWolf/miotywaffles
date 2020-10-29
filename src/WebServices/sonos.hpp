/**
 * @file sonos.hpp
 * @author Daniel K. Vinther Wolf
 * @brief SonosControl
 * @version 0.1
 * @date 2020-09-23
 * 
 * IFTTT Sonos API
 */

#include "Particle.h"

#ifndef _SONOS_HPP_
#define _SONOS_HPP_

class SonosControl
{
public:
    /* Attributes */

    /* Methods */
    SonosControl();
    ~SonosControl();
    bool SonosPlay(String state);
    bool ConnectedToCloud(void);
    void ConnectToCloud(void);

private:
    /* Attributes */
    const String _event = "waffleiron_sonos";
    const String _states[3] = {"heating", "ready", "baking"};
    bool _connectedToCloud;
    bool _publishToCloudSuccess;
};

#endif
