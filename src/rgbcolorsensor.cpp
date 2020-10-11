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
#include "rgbcolorsensor.hpp"

RgbColorSensor::RgbColorSensor()
{
    // From Particle.h, set up I2C:
    Wire.setSpeed(CLOCK_SPEED_100KHZ);
    if (!Wire.isEnabled())
    {
        Wire.begin(); // We are master on the bus!

        // Enable internal clock etc. in the sensor
        this->initSensor();
    }
}

RgbColorSensor::~RgbColorSensor()
{
    // do nothing
}

void RgbColorSensor::initSensor()
{

    Wire.beginTransmission(WireTransmission(RGB_SENSOR_ADDR).timeout(200ms));
    Wire.write(COMMAND_REG);
    // Write data: 0x03 to enable internal clock, gain=0, etc.
    Wire.write(DATA_RGB_SENSOR_ENABLE);
    Wire.endTransmission();
}
bool RgbColorSensor::sensorConnected()
{
    Wire.beginTransmission(WireTransmission(RGB_SENSOR_ADDR).timeout(200ms));
    Wire.write(COMMAND_REG | DEVICE_ID);
    Wire.endTransmission();

    Wire.requestFrom(RGB_SENSOR_ADDR, 1);
    this->_deviceID = (int)Wire.read();
    if (this->_deviceID == int(0x44))
    {
        return true;
    }
    else
    {
        return false;
    }
    // Device ID 0x44 in SubAddress 0xB2, return true:
}

String RgbColorSensor::getColor()
{
    // get red and green value and return
    // dominant color as string

    //
}

int RgbColorSensor::getGreenValue()
{
    // Get green low + high:
    // low(reg:0xB8) High(reg:0xB9)
}

int RgbColorSensor::getRedValue()
{
    // Get red low + high:
    // low(reg:0xB6) High(reg:0xB7)
}