/***************************************************************************/ /**
 * 
 * Source File	: 	rgbcolorsensor.cpp
 * Author		: 	Daniel K. Vinther Wolf 
 * Created		:	20201009
 * Version		:	0.1.0
 * 
 * Description	:	I2C Communication with the RGB Color Sensor
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

class RgbColorSensor
{

public:
    /* Attributes */
    int _deviceID;
    int _redValue;
    int _greenValue;
    /* Methods */
    RgbColorSensor();
    ~RgbColorSensor();
    bool sensorConnected();
    int getColor();

private:
    /* Attributes */

    /* Methods */
    void initSensor();
    int getColorValue();
};

#endif