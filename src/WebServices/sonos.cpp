/**
 * @file sonos.cpp
 * @author Daniel K. Vinther Wolf
 * @brief IFTTT Sonos API
 * @version 0.1
 * @date 2020-09-23
 * 
 * 
 */

#include "Particle.h"
#include "sonos.hpp"

SonosControl::SonosControl()
{
    _publishToCloudSuccess = false;
}

SonosControl::~SonosControl()
{
    //
}

/**
 * @brief Send event to IFTTT and make Sonos change music accordingly
 *  
 * @param state 
 * @return true 
 * @return false 
 * 
 * If state is:
 *  "heating" play: Guns'n'Roses - Patience
 *  "ready" play: AC/DC - Let me put my love into you
 *  "baking": Mark Ronson ft. Bruno Mars - Uptown Funk
 *             
 */
bool SonosControl::SonosPlay(String state)
{
    _publishToCloudSuccess = Particle.publish(_event, state, PUBLIC);
    return _publishToCloudSuccess;
}

const String SonosControl::_event = "waffleiron_sonos";
const String SonosControl::_states[3] = {"heating", "ready", "baking"};