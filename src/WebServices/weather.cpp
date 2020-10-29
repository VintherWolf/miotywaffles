/**
 * @file weather.cpp
 * @author Daniel K. Vinther Wolf
 * @brief Use OpenWeatherMap API to retrive weather reports
 * @version 0.1
 * @date 2020-10-23
 * 
 * 
 */

#include "weather.hpp"
#include "Particle.h"

Weather::Weather()
{
    // Setup Current Weather Webhook data params
    this->city_name = "Aarhus";

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

/**
 * @brief Retrieve weather report for Aarhus
 * 
 */
void Weather::getWeatherReport()
{
    // The Response is obtained in receivedWeatherReport Event
    Particle.publish("miotywaffles_owm_report", this->city_name, PRIVATE);
}

/**
 * @brief Parse weather report and check if weather is good or bad
 * 
 * @param data 
 */
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