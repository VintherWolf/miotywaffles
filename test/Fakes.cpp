#include "../src/sonos.hpp"

class FakeParticleCloud : public SonosControl
{

public:
    FakeParticleCloud();
    void SonosPlay(std::string state);
    bool ConnectedToCloud(void);
    void ConnectToCloud(void);
    void ShowPossibleStates(void);
    //
private:
    int CallsToConnectedToCloud;
};

FakeParticleCloud::FakeParticleCloud()
{
    CallsToConnectedToCloud = 0;
    _connectedToCloud = false;
}

void FakeParticleCloud::SonosPlay(std::string state)
{
    std::cout << state << std::endl;
}

bool FakeParticleCloud::ConnectedToCloud()
{
    CallsToConnectedToCloud++;
    return _connectedToCloud;
}

void FakeParticleCloud::ConnectToCloud()
{
    _connectedToCloud = true;
}

void FakeParticleCloud::ShowPossibleStates()
{
    //
}