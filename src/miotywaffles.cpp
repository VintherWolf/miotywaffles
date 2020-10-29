/***************************************************************************/ /**
 * 
 * Source File	: 	miotywaffles.cpp
 * Author		: 	Daniel K. Vinther Wolf 
 * Created		:	20200917
 * Version		:	0.3.2
 * 
 * Description	:	MiotyWaffles
 * 
 * 
 ******************************************************************************/
#include "Particle.h"

// Settings:
#include "./Settings/pinsettings.h"
// Sensors:
#include "./Sensors/rgbcolorsensor.hpp"
#include "./Sensors/anglesensor.h"
#include "./Sensors/touchsensor.h"
// Actuators:
#include "./Actuators/buzzer.hpp"
#include "./Actuators/relay.h"
// Web Services:
#include "./WebServices/weather.hpp"
#include "./WebServices/sonos.hpp"
#pragma region Constants
/* Constants */
// Max retries to see if Waffle Iron is powered on
#define MAX_RETRIES 12

// Set Baking time in Minutes and make it run only once
#define SET_BAKING_TIME (5min)
#define ONE_SHOT true
#pragma endregion Constants

#pragma region initialization
/**
*=======================================================
* Initialization of Shared Variables
*=======================================================
**/
// System Sleeps between program runs
SystemSleepConfiguration config;

// Log Handler
SerialLogHandler logHandler;

// Sonos API
SonosControl sc;

// RGB Color Sensor
RgbColorSensor rgb;

// Buzzer
Buzzer buzzer;

// Forward Definitions
void miotyWaffles();
void toggleLed(uint16_t pin);

// Events:
void recievedWeatherReport(const char *event, const char *data);

// Open Weather Map WebService:
Weather weather;

// Baking and "baking watchdog" Timer setup:
void bakingIsDone();
void stillBaking();
Timer bakingTimer(20000, bakingIsDone, ONE_SHOT);
Timer bakeWatchdogTimer(5000, stillBaking);
bool doneBaking = false;

#pragma endregion init

#pragma region setup()
/***********************************************************************
 *  @brief setup() runs once when the device is first turned on.
 ***********************************************************************/
void setup()
{
    // Initialize Pinsettings (refer to pinsettings.h for pinout):
    initAngleSensor();
    initTouchSensor();
    initRelay();

    // Argon On-board Blue LED
    pinMode(BLUE_LED, OUTPUT);
    toggleLed(BLUE_LED);

    // Configure system to wake from sleep by Touch Sensor
    config.mode(SystemSleepMode::STOP)
        .gpio(TOUCH_SENSOR, RISING);

    // Syncronize and validate time with Particle cloud
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

    // Subscribe to Weather Report response
    Particle.subscribe("hook-response/miotywaffles_owm_report", recievedWeatherReport, MY_DEVICES);
}
#pragma endregion setup()

#pragma region loop()
/***********************************************************************
 *  @brief loop() runs over and over again, as quickly as it can execute.
 ***********************************************************************/
void loop()
{
    // Sleeps until Touch Sensor is activated
    delay(5000);
    buzzer.playAngryBirds();
    Log.info("%s Goind to sleep! Activate touch sensor to wake up",
             Time.timeStr().c_str());

    System.sleep(config);
    buzzer.playWokeUp();
    Log.info("%s Touch Sensor Activated! Awake now",
             Time.timeStr().c_str());

    // Run Program
    miotyWaffles();
}
#pragma endregion loop()

#pragma region MiotyWaffles(Application)
/***********************************************************************
 *  @brief MiotyWaffles 
 ***********************************************************************/
void miotyWaffles()
{
#pragma region mioty init
    int retry = 0;
    delay(10000);
    // Initialize RGB Sensor
    if (rgb.sensorIsConnected())
    {
        Log.info("%s RGB Sensor Initialized %d", Time.timeStr().c_str(), rgb.deviceID);
        delay(1000);
    }
    else
    {
        Log.error("%s RGB SENSOR NOT PRESENT", Time.timeStr().c_str());
        //exit(EXIT_FAILURE);
    }

    Log.info("%s Get Weather Report", Time.timeStr().c_str());
    weather.getWeatherReport();

    // Enable Waffle Iron by turning Relay On
    setRelay("on");
    delay(2000);

    // Wait for LED to be Red/Orange
    while (rgb.getColor() != Red)
    {
        ++retry;
        delay(5000);
        toggleLed(BLUE_LED);
        Log.info("%s Colors: Red=%d Green=%d Diff=%d", Time.timeStr().c_str(),
                 rgb.redValue, rgb.greenValue, rgb.redValue - rgb.greenValue);

        // Exit if LED is not RED/Orange within MAX_RETRIES
        if (retry >= MAX_RETRIES)
        {
            Log.error("Waffle Iron is not powered ON!");
            digitalWrite(RELAY_WIRON, LOW);
            digitalWrite(BLUE_LED, LOW);
            return;
        }
    }
#pragma endregion mioty init
#pragma region State Heatting
    /**
    *=======================================================
    * STATE: Heating
    *=======================================================
    **/
    Log.info("%s State is now: Heating", Time.timeStr().c_str());

    // Change Track on Sonos System
    if (!sc.SonosPlay("heating"))
    {
        Log.warn("%s Connection Lost: Music cannot play!",
                 Time.timeStr().c_str());
        // TO-DO: Buzzer ->> Beep Beep
    }
    Log.info("%s Music Should Change About Now!",
             Time.timeStr().c_str());

    // Wait for WaffleIron LED to be Green
    retry = 0;
    while (rgb.getColor() != Green)
    {
        ++retry;
        Log.info("%s Colors: Red=%d Green=%d Diff=%d", Time.timeStr().c_str(),
                 rgb.redValue, rgb.greenValue, rgb.redValue - rgb.greenValue);
        delay(5000);
        toggleLed(BLUE_LED);
        if (retry >= 60)
        {
            Log.error("Took too long to heat up!");
            return;
        }
    }
#pragma endregion state Heating
#pragma region State Ready
    /**
    *=======================================================
    * STATE: Ready
    *=======================================================
    **/
    Log.info("%s State is now: Ready", Time.timeStr().c_str());

    // Change Track on Sonos System
    if (!sc.SonosPlay("ready"))
    {
        Log.warn("%s Connection Lost: Music cannot play!",
                 Time.timeStr().c_str());
        // Buzzer ->> Beep Beep
    }
    Log.info("%s Music Should Change About Now!", Time.timeStr().c_str());

    // Wait for WaffleIron Lid to open:
    while (!lidIsOpen())
    {
        toggleLed(BLUE_LED);
        delay(1000);
    }
    Log.info("%s Lid was Opened!", Time.timeStr().c_str());

    // Wait for WaffleIron Lid to close:
    while (lidIsOpen())
    {
        toggleLed(BLUE_LED);
        delay(200);
    }
    Log.info("%s Lid was Closed!", Time.timeStr().c_str());
#pragma endregion state Ready
#pragma region State Baking
    /**
    *=======================================================
    * STATE: Baking
    *=======================================================
    **/
    Log.info("%s State is now: Baking", Time.timeStr().c_str());

    // Start Baking timer/
    //bakingTimer.changePeriod(SET_BAKING_TIME);
    bakeWatchdogTimer.start();
    bakingTimer.start();

    // Change Track on Sonos System
    if (!sc.SonosPlay("baking"))
    {
        Log.warn("%s Connection Lost: Music cannot play!",
                 Time.timeStr().c_str());
        // Buzzer ->> Beep Beep
    }
    Log.info("%s Music Should Change About Now!", Time.timeStr().c_str());
#pragma endregion State baking
#pragma region Process is Done
    while (!doneBaking)
    {
        // wait for baking timer to run out
        delay(10);
    }

    // TO-DO: Turn off Relay
    digitalWrite(RELAY_WIRON, LOW);
    // TO-DO: Check that Relay turned off (no LED light from waffleiron)
    delay(300);
    if (!rgb.getColor() == noChange)
    {
        Log.warn("%s Tries to turn off Relay one more time!", Time.timeStr().c_str());
        digitalWrite(RELAY_WIRON, LOW);
        delay(1000);
        if (!rgb.getColor() == noChange)
        {
            Log.error("%s Relay did not turn off!", Time.timeStr().c_str());
            exit(EXIT_FAILURE);
        }
    }
    delay(2000);
    Log.info("%s All Done! Have an Enjoyable Day!", Time.timeStr().c_str());
    digitalWrite(BLUE_LED, LOW);
    return;
}
#pragma endregion Process is Done
#pragma endregion MiotyWaffles(Application)

#pragma region support functions and events
// When baking timer runs out:
void bakingIsDone()
{
    Log.info("%s Baking Timer Ran out", Time.timeStr().c_str());
    doneBaking = true;
    bakingTimer.stop();
}

// Baking timer "watchdog" timer
void stillBaking()
{
    if (!doneBaking)
    {
        Log.info("%s Still baking", Time.timeStr().c_str());
        toggleLed(BLUE_LED);
    }
    else
    {
        bakeWatchdogTimer.stop();
    }
}

// Subscribe to weather report response
void recievedWeatherReport(const char *event, const char *data)
{
    // Check if it is Good or Bad Weather from OpenWeatherMap JSON Response
    weather.evaluateReport(data);
}

// helper function to toggle a GPO pin
void toggleLed(uint16_t pin)

{
    if (getPinMode(pin) == OUTPUT)
    {
        if (digitalRead(pin) == LOW)
        {
            digitalWrite(pin, HIGH);
        }
        else
        {
            digitalWrite(pin, LOW);
        }
    }
}
#pragma endregion support functions and events