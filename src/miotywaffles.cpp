/***************************************************************************/ /**
 * 
 * Source File	: 	miotywaffles.cpp
 * Author		: 	Daniel K. Vinther Wolf 
 * Created		:	20200917
 * Version		:	0.2.1
 * 
 * Description	:	MiotyWaffles
 * 
 * 
 ******************************************************************************/

#include "Particle.h"

#include "pinsettings.h"
#include "sonos.hpp"
#include "rgbcolorsensor.hpp"

/**
*=======================================================
* Initialization of Shared Variables
*=======================================================
**/
// Log Handler
SerialLogHandler logHandler;

// Sonos API
SonosControl sc;

// RGB Color Sensor
RgbColorSensor rgb;
bool rgbSensorConnected = false;
int RgbColor = 0;

// Time Logging
//time_t time = Time.now();
//Time.format(time, TIME_FORMAT_ISO8601_FULL);

/***********************************************************************
 *  @brief setup() runs once when the device is first turned on.
 ***********************************************************************/
void setup()
{
    delay(10000);
    /**
    *=======================================================
    * Process Start, Setting up 
    *=======================================================
    **/
    Log.info("%s Syncronizing Time with Particle Cloud!",
             Time.timeStr().c_str());
    Particle.syncTime();

    waitUntil(Particle.syncTimeDone);
    Log.info("%s Waiting for time to be Valid", Time.timeStr().c_str());
    waitFor(Time.isValid, 30000);

    if (Time.isValid())
    {
        Log.info("%s Time is Valid. Waffle Iron Process Started!",
                 Time.timeStr().c_str());
    }
}

/***********************************************************************
 *  @brief loop() runs over and over again, as quickly as it can execute.
 ***********************************************************************/
void loop()
{

    delay(10000);
    Log.info("%s I2C Test", Time.timeStr().c_str());
    rgbSensorConnected = false;
    rgbSensorConnected = rgb.sensorConnected();

    if (rgbSensorConnected)
    {
        Log.info("%s RGB SENSOR CONNECTED %d", Time.timeStr().c_str(), rgb._deviceID);
    }
    else
    {
        Log.info("%s RGB SENSOR NOT PRESENT", Time.timeStr().c_str());
    }

    for (int i = 0; i < 100; i++)
    {

        RgbColor = rgb.getColor();
        Log.info("%s Reading Number: %d", Time.timeStr().c_str(), i);
        Log.info("%s Color is %d", Time.timeStr().c_str(), RgbColor);

        Log.info("%s Colors: Red=%d Green=%d Diff=%d", Time.timeStr().c_str(), rgb._redValue, rgb._greenValue, rgb._redValue - rgb._greenValue);
        delay(5000);
    }

    Log.info("%s QUIT APP Now", Time.timeStr().c_str());

    delay(900000);
}

void miotyWaffles()
{
    delay(20000);
    // TO-DO: Enable Waffle Iron by turning Relay On

    // TO-DO: Verify Waffle Iron turned ON (LED = Orange)

    /**
    *=======================================================
    * STATE: Heating
    *=======================================================
    **/
    Log.info("%s State is now: Heating", Time.timeStr().c_str());

    if (!sc.SonosPlay("heating"))
    {
        Log.info("%s Connection Lost: Music cannot play!",
                 Time.timeStr().c_str());
        // TO-DO: Buzzer ->> Beep Beep
    }
    Log.info("%s Music Should Change About Now!",
             Time.timeStr().c_str());
    // TO-DO: Wait for WaffleIron LED to be Green
    // Fast Paced delay to simulate Waffleiron
    delay(60000);
    /**
    *=======================================================
    * STATE: Ready
    *=======================================================
    **/
    Log.info("%s State is now: Ready", Time.timeStr().c_str());

    if (!sc.SonosPlay("ready"))
    {
        Log.info("%s Connection Lost: Music cannot play!",
                 Time.timeStr().c_str());
        // Buzzer ->> Beep Beep
    }
    Log.info("%s Music Should Change About Now!", Time.timeStr().c_str());
    // TO-DO: Wait for WaffleIron Lid to open and close again
    // Fast Paced delay to simulate Waffleiron
    delay(60000);
    /**
    *=======================================================
    * STATE: Baking
    *=======================================================
    **/
    Log.info("%s State is now: Baking", Time.timeStr().c_str());
    // TO-DO: Set timer to x minutes y seconds
    // Fast Paced delay to simulate Waffleiron
    if (!sc.SonosPlay("baking"))
    {
        Log.info("%s Connection Lost: Music cannot play!",
                 Time.timeStr().c_str());
        // Buzzer ->> Beep Beep
    }
    Log.info("%s Music Should Change About Now!", Time.timeStr().c_str());
    // TO-DO: Wait for timer to run out
    // Fast Paced delay to simulate Waffleiron
    delay(10000);

    // TO-DO: Turn off Relay

    // TO-DO: Check that Relay turned off (no LED light from waffleiron)

    delay(60000);
}