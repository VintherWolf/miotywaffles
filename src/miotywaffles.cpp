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

#define TIME_LIMIT 12

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

// Forward Definitions
void miotyWaffles();
bool lidIsOpen();

/***********************************************************************
 *  @brief setup() runs once when the device is first turned on.
 ***********************************************************************/
void setup()
{
    delay(10000);
    // Initialize Pinsettings (refer to pinsettings.h for pinout):
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

    // Initialize RGB Sensor
    rgbSensorConnected = rgb.sensorConnected();
    if (rgbSensorConnected)
    {
        Log.info("%s RGB Sensor Initialized %d", Time.timeStr().c_str(), rgb.deviceID);
        delay(1000);
    }
    else
    {
        Log.info("%s RGB SENSOR NOT PRESENT", Time.timeStr().c_str());
        exit(EXIT_FAILURE);
    }

    // Check Color of WaffleIron LED
    RgbColor = rgb.getColor();
    Log.info("%s Color is %d", Time.timeStr().c_str(), RgbColor);
    Log.info("%s Colors: Red=%d Green=%d Diff=%d", Time.timeStr().c_str(),
             rgb.redValue, rgb.greenValue, rgb.redValue - rgb.greenValue);

    // Wait for LED to be Red/Orange
    int elapsedTime = 0;
    while (RgbColor != Red)
    {
        ++elapsedTime;
        delay(5000);

        // Check Color of WaffleIron LED
        RgbColor = rgb.getColor();
        Log.info("%s Color is %d", Time.timeStr().c_str(), RgbColor);
        Log.info("%s Colors: Red=%d Green=%d Diff=%d", Time.timeStr().c_str(),
                 rgb.redValue, rgb.greenValue, rgb.redValue - rgb.greenValue);

        // Exit if LED is not RED/Orange within Time_limit
        if (elapsedTime >= TIME_LIMIT)
        {
            Log.info("Waffle Iron is not powered ON!");
            exit(EXIT_FAILURE);
        }
    }

    /**
    *=======================================================
    * STATE: Heating
    *=======================================================
    **/
    Log.info("%s State is now: Heating", Time.timeStr().c_str());

    // Change Track on Sonos System
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

    // Change Track on Sonos System
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

    // Change Track on Sonos System
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
    exit(EXIT_SUCCESS);
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