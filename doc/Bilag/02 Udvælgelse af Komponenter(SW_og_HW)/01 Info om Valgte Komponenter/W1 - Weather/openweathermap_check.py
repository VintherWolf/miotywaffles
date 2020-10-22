#!/usr/env python
# -*- coding: utf-8 -*-
"""
    Author:         Daniel K. Vinther Wolf
    Created:        22-10-2020
    Description:    Use OpenWeatherMap to Retrieve local weather data
    Dependencies:   API Key from OpenWeatherMap (Account)
    Pre-Requisites: API Key shall be stored in env.var (OPENWEATHERKEY=key)

"""
__author__ = "Daniel Korsgaard Vinther Wolf"
__version__ = "1.0.0"

import os
import requests

# Input:
city_name = "Aarhus"
api_key = os.environ['OPENWEATHERKEY']

# Use Open Weather Map API to get report for city_name
'''
Retrieve local weather report (GET String format):
api.openweathermap.org/data/2.5/weather?q={city name}&appid={API key}
'''

# Use JSON format:
url = "https://api.openweathermap.org/data/2.5/weather"
data = {"q": city_name, "appid": api_key}


def convert_to_celcius(temp_K):
    return round(temp_K-273.15)


if __name__ == "__main__":
    print("This will send a request to open weather map "
          "and request weather data for city name: "+city_name+"\n")

    # Send request and check if response is OK
    response = requests.get(url, data)

    if response.status_code == 200:
        weather_report = response.json()
    else:
        print("Failed with response code:", response.status_code)

    # Collect "Weather.main", "main.temp", "main.humidity"
    weather_headline = weather_report['weather'][0]['main']

    temperature = weather_report['main']['temp']
    temperature = convert_to_celcius(temperature)

    humidity = weather_report['main']['humidity']

    # show weather report findings:
    print("Weather Report for", city_name, ":\n")
    print("Headline:", weather_headline,
          "\nTemperature [C]: ", temperature,
          "\nHumidity [%]: ", humidity)
