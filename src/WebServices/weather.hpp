/***************************************************************************/ /**
 * 
 * Source File	: 	weather.hpp
 * Author		: 	Daniel K. Vinther Wolf 
 * Created		:	20201023
 * Version		:	0.1.0
 * 
 * Description	:	OpenWeatherMap API
 * 
 * 
 ******************************************************************************/

#include "Particle.h"

#ifndef _WEATHER_HPP_
#define _WEATHER_HPP_

class IWeather
{
public:
    virtual void getWeatherReport();

private:
};

// Conrete Weather Class
class Weather : public IWeather
{
public:
    String city_name;
    Weather();
    ~Weather();
    void getWeatherReport();
    void evaluateReport(const char *data);

private:
    String _apiKey;
    bool _weatherIsGood;
};

#endif