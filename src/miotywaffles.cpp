/***************************************************************************/ /**
 * 
 * Source File	: 	miotywaffles.cpp
 * Author		: 	Daniel K. Vinther Wolf 
 * Created		:	20200917
 * Version		:	0.3.0
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

// Forward Definitions
void miotyWaffles();
bool isLidClosed();

/***********************************************************************
 *  @brief setup() runs once when the device is first turned on.
 ***********************************************************************/
void setup()
{
    delay(10000);
    // Pins (refer to pinsettings.h):
    pinMode(ANGLE_SENSOR, INPUT);
    pinMode(TOUCH_SENSOR, INPUT);

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
        Log.info("%s Time is Valid! Waffle Iron Process Started!",
                 Time.timeStr().c_str());
    }

    miotyWaffles();
}

/***********************************************************************
 *  @brief loop() runs over and over again, as quickly as it can execute.
 ***********************************************************************/
void loop()
{

    //
}

void miotyWaffles()
{
    delay(20000);
    // TO-DO: Enable Waffle Iron by turning Relay On

    // Verify Waffle Iron turned ON (LED = Orange)
    rgbSensorConnected = rgb.sensorConnected();

    if (rgbSensorConnected)
    {
        Log.info("%s RGB Sensor Initialized %d", Time.timeStr().c_str(), rgb._deviceID);
        delay(1000);
    }
    else
    {
        Log.info("%s RGB SENSOR NOT PRESENT", Time.timeStr().c_str());
        EXIT_FAILURE;
    }

    RgbColor = rgb.getColor();

    Log.info("%s Color is %d", Time.timeStr().c_str(), RgbColor);
    Log.info("%s Colors: Red=%d Green=%d Diff=%d", Time.timeStr().c_str(),
             rgb._redValue, rgb._greenValue, rgb._redValue - rgb._greenValue);

    int timeout = 0;
    while (RgbColor != Red)
    {
        ++timeout;
        delay(5000);
        RgbColor = rgb.getColor();
        Log.info("%s Color is %d", Time.timeStr().c_str(), RgbColor);
        Log.info("%s Colors: Red=%d Green=%d Diff=%d", Time.timeStr().c_str(),
                 rgb._redValue, rgb._greenValue, rgb._redValue - rgb._greenValue);

        if (timeout > 10 && RgbColor != Green)
        {
            Log.info("Waffle Iron is not powered ON!");
            EXIT_FAILURE;
        }
    }

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

    // Wait for WaffleIron LED to be Green
    while (RgbColor != Green)
    {
        RgbColor = rgb.getColor();
        delay(5000);
    }
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

    // Wait for WaffleIron Lid to open:
    while (!lidIsOpen())
    {
        delay(1000);
    }
    Log.info("%s Lid was Opened!", Time.timeStr().c_str());

    // Wait for WaffleIron Lid to close:
    while (lidIsOpen())
    {
        delay(1000);
    }
    Log.info("%s Lid was Closed!", Time.timeStr().c_str());
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
    Log.info("%s All Done! Have an Enjoyable Day!", Time.timeStr().c_str());
    EXIT_SUCCESS;
}

bool lidIsOpen()
{
    if (digitalRead(ANGLE_SENSOR) == LOW)
    {
        // Lid is OPENED when input is LOW
        // Refer to Electrical Diagram
        return true;
    }

    else
    {
        return false;
    }
}