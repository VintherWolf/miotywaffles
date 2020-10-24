/***************************************************************************/ /**
 * 
 * Source File	: 	weather.cpp
 * Author		: 	Daniel K. Vinther Wolf 
 * Created		:	20201023
 * Version		:	0.1.0
 * 
 * Description	:	OpenWeatherMap API
 * 
 * 
 ******************************************************************************/

#include "weather.hpp"
#include "Particle.h"
#include "../Settings/OWM_API_key.h"

Weather::Weather()
{
    // Setup
    this->city_name = "Aarhus";
    this->_apiKey = owmApiKey;
    this->_weatherIsGood = false;
}

Weather::~Weather()
{
    // Tear down
}

void Weather::getWeatherReport()
{
    // The Response is obtained in receivedWeatherReport Event
    Particle.publish("miotywaffles_owm_report", this->city_name, PRIVATE);
}
void Weather::evaluateReport(const char *data)
{
    Log.info("%s Received Weather Report", Time.timeStr().c_str());

    String goodWeather[] = {"clouds",
                            "clear",
                            "clear sky"};

    JSONValue outerObj = JSONValue::parseCopy(data);
    this->_weatherIsGood = false;
    JSONObjectIterator iter(outerObj);
    while (iter.next())
    {
        for (int i = 0; i < 3; i++)
        {
            if (iter.name() == goodWeather[i])
            {
                Log.info("The Weather is good!");
                this->_weatherIsGood = true;
            }
        }
    }
    if (!this->_weatherIsGood)
    {
        Log.info("The weather is bad!");
    }
}