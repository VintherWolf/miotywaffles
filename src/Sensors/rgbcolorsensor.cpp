/***************************************************************************/ /**
 * 
 * Source File	: 	rgbcolorsensor.cpp
 * Author		: 	Daniel K. Vinther Wolf 
 * Created		:	20201009
 * Version		:	0.1.1
 * 
 * Description	:	I2C Communication with the RGB Color Sensor
 *
 * 
 ******************************************************************************/
#include "Particle.h"
#include "rgbcolorsensor.hpp"

RgbColorSensor::RgbColorSensor()
{
    Colors_t colors = noChange;

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

    this->_isConnected = false;
}

bool RgbColorSensor::sensorIsConnected()
{
    Wire.beginTransmission(WireTransmission(RGB_SENSOR_ADDR).timeout(200ms));
    Wire.write(COMMAND_REG | DEVICE_ID);
    Wire.endTransmission();

    Wire.requestFrom(RGB_SENSOR_ADDR, 1);
    this->deviceID = (int)Wire.read();
    if (this->deviceID == int(0x44))
    {
        this->_isConnected = true;
    }
    else
    {
        this->_isConnected = false;
    }
    return this->_isConnected;
    // Device ID 0x44 in SubAddress 0xB2, return true:
}

int RgbColorSensor::getColor()
{
    int diff = 0;
    this->getColorValue();

    diff = this->redValue - this->greenValue;

    // diff: Negative => Green is dominant,
    //       Positive => Red is dominant
    if (diff > COLOR_HYSTERESIS)
    {
        // Red is dominant
        this->_currentColor = Red;
    }
    else if (diff < -COLOR_HYSTERESIS)
    {
        // Green is dominant
        this->_currentColor = Green;
    }
    else
    {
        // No dominant color
        this->_currentColor = noChange;
    }
    return this->_currentColor;
}

int RgbColorSensor::getColorValue()
{
    // Get green low + high = 2bytes:
    // low(reg:0xB8) High(reg:0xB9)
    // Get red low + high = 2bytes:
    // low(reg:0xB6) High(reg:0xB7)

    /* Get Red Value */
    Wire.beginTransmission(WireTransmission(RGB_SENSOR_ADDR).timeout(200ms));
    Wire.write(COMMAND_REG | RED_LOW);
    Wire.endTransmission();
    Wire.requestFrom(RGB_SENSOR_ADDR, 2);
    this->redValue = (int)Wire.read();

    /* Get Green Value */
    Wire.beginTransmission(WireTransmission(RGB_SENSOR_ADDR).timeout(200ms));
    Wire.write(COMMAND_REG | GREEN_LOW);
    Wire.endTransmission();
    Wire.requestFrom(RGB_SENSOR_ADDR, 2);
    this->greenValue = (int)Wire.read();
}