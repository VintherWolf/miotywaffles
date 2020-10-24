/***************************************************************************/ /**
 * 
 * Source File	: 	weather.cpp
 * Author		: 	Daniel K. Vinther Wolf 
 * Created		:	20201023
 * Version		:	0.1.1
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
    // Setup Current Weather Webhook data params
    this->city_name = "Aarhus";
    this->_apiKey = owmApiKey;

    // List of possible labels for "good weather"
    this->_goodWeather[0] = "clouds";
    this->_goodWeather[1] = "clear";
    this->_goodWeather[2] = "clear sky";

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
    // Parse Received JSON-formated weather report
    JSONValue outerObj = JSONValue::parseCopy(data);
    JSONObjectIterator iter(outerObj);

    // WeahterIsGood will be flipped, if evaluations deems it as good
    this->_weatherIsGood = false;
    while (iter.next())
    {
        for (int i = 0; i < 3; i++)
        {
            // if headline for weather sound good, then weather is good
            if (iter.name() == this->_goodWeather[i])
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