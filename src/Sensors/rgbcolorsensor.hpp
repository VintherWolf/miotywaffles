/***************************************************************************/ /**
 * 
 * Source File	: 	rgbcolorsensor.cpp
 * Author		: 	Daniel K. Vinther Wolf 
 * Created		:	20201009
 * Version		:	0.1.0
 * 
 * Description	:	I2C Communication with the RGB Color Sensor 
 *                  (ADAfruit 1334)
 *
 * 
 ******************************************************************************/
#include "Particle.h"

#ifndef _RGBCOLORSENSOR_HPP_
#define _RGBCOLORSENSOR_HPP_

#define RGB_SENSOR_ADDR (0x29)
#define COMMAND_REG (0xA0)

#define DATA_RGB_SENSOR_ENABLE (0x03) // SubAddress

#define DEVICE_ID (0x12)

#define RED_LOW (0x16)
#define RED_HIGH (0x17)

#define GREEN_LOW (0x18)
#define GREEN_HIGH (0x19)

#define COLOR_HYSTERESIS 20

enum Colors_t
{
    noChange,
    Red,
    Green
};

class RgbColorSensor
{

public:
    /* Attributes */
    int deviceID;
    int redValue;
    int greenValue;
    /* Methods */
    RgbColorSensor();
    ~RgbColorSensor();
    bool sensorIsConnected();
    int getColor();

private:
    /* Attributes */
    bool _isConnected;
    int _currentColor;

    /* Methods */
    void initSensor();
    int getColorValue();
};

#endif