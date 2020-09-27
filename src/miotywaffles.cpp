/***************************************************************************/ /**
 * 
 * Source File	: 	miotywaffles.cpp
 * Author		: 	Daniel K. Vinther Wolf 
 * Created		:	20200917
 * Version		:	0.2.0
 * 
 * Description	:	MiotyWaffles
 * 
 * 
 ******************************************************************************/

#include "Particle.h"

#include "pinsettings.h"
#include "sonos.hpp"

/**
*=======================================================
* Initialization of Shared Variables
*=======================================================
**/
// Log Handler
SerialLogHandler logHandler;

// Sonos API
SonosControl sc;

// Time Logging
//time_t time = Time.now();
//Time.format(time, TIME_FORMAT_ISO8601_FULL);

/***********************************************************************
 *  @brief setup() runs once when the device is first turned on.
 ***********************************************************************/
void setup()
{
    /**
    *=======================================================
    * Process Start, Setting up 
    *=======================================================
    **/
    Particle.syncTime();
    waitUntil(Particle.syncTimeDone);

    waitFor(Time.isValid, 30000);
    if (Time.isValid())
    {
        Log.info("%s Waffle Iron Process Started!", Time.timeStr().c_str());
    }

    /**
    *=======================================================
    * STATE: Heating
    *=======================================================
    **/
    Log.info("%s State is now: Heating", Time.timeStr().c_str());

    if (!sc.SonosPlay("heating"))
    {
        // Buzzer ->> Beep Beep
    }

    // Fast Paced delay to simulate Waffleiron
    delay(10000);
    /**
    *=======================================================
    * STATE: Ready
    *=======================================================
    **/
    Log.info("%s State is now: Ready", Time.timeStr().c_str());

    if (!sc.SonosPlay("ready"))
    {
        // Buzzer ->> Beep Beep
    }

    // Fast Paced delay to simulate Waffleiron
    delay(10000);
    /**
    *=======================================================
    * STATE: Baking
    *=======================================================
    **/
    Log.info("%s State is now: Baking", Time.timeStr().c_str());

    if (!sc.SonosPlay("baking"))
    {
        // Buzzer ->> Beep Beep
    }

    // Fast Paced delay to simulate Waffleiron
    delay(10000);
}

/***********************************************************************
 *  @brief loop() runs over and over again, as quickly as it can execute.
 ***********************************************************************/
void loop()
{
    delay(30000);
    Log.info("Sending Heating");
    sc.SonosPlay("heating");
    Log.info("Done");
}