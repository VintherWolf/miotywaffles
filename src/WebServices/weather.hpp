/**
 * @file weather.hpp
 * @author Daniel K. Vinther Wolf
 * @brief Weather
 * @version 0.1
 * @date 2020-10-23
 * 
 * Use OpenWeatherMap API to retrive weather reports
 */

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
    String _goodWeather[3];
    bool _weatherIsGood;
};

#endif