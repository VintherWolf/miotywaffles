/**
 * @file rgbcolorsensor.hpp
 * @author Daniel K. Vinther Wolf
 * @brief RgbColorSensor
 * @version 0.1
 * @date 2020-10-09
 * 
 * Description: I2C Communication with the RGB Color Sensor
 *      Device: Adafruit 1334
 */

#include "Particle.h"

#ifndef _RGBCOLORSENSOR_HPP_
#define _RGBCOLORSENSOR_HPP_

// Device Slave Address
#define RGB_SENSOR_ADDR (0x29)

// The following definitions are registers that are used:
#define COMMAND_REG (0xA0)
// Write 0x03 to Command register to initialize device (gain=0 etc.)
#define DATA_RGB_SENSOR_ENABLE (0x03)
// Register for Device ID:
#define DEVICE_ID (0x12)
// Registers for low and High byte values for Red color
#define RED_LOW (0x16)
#define RED_HIGH (0x17)
// Registers for low and High byte values for green color
#define GREEN_LOW (0x18)
#define GREEN_HIGH (0x19)
// Color hysteresis is used to enhance noise immunity to prevent it
// from flipping between red and green due to noise.
#define COLOR_HYSTERESIS 20

// Color states, where noChange is in the grey 'hysteresis area'
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