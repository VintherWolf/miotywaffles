/***************************************************************************/ /**
 * 
 * Source File	: 	sonos.hpp
 * Author		: 	Daniel K. Vinther Wolf 
 * Created		:	20200923
 * Version		:	0.1.0
 * 
 * Description	:	Sonos API
 * 
 * 
 ******************************************************************************/

#ifndef _SONOS_HPP_
#define _SONOS_HPP_

#include <string>
#include <iostream>

class SonosControl
{

public:
    const std::string _event = "waffleiron_sonos";
    const std::string _states[3] = {"heating", "ready", "baking"};
    bool _connectedToCloud;
    SonosControl();
    ~SonosControl();
    virtual void SonosPlay(std::string state) = 0;
    virtual bool ConnectedToCloud(void) = 0;
    virtual void ConnectToCloud(void) = 0;
    virtual void ShowPossibleStates(void) = 0;

private:
    //
};

// Interface Sonos with Particle.h
class ParticleCloud : public SonosControl
{
public:
    void SonosPlay(std::string state);
    bool ConnectedToCloud(void);
    void ConnectToCloud(void);
    void ShowPossibleStates(void);
    //
private:
    //
};

#endif // _SONOS_HPP_
