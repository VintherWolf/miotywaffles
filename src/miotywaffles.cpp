/**
 * @file miotywaffles.cpp
 * @author Daniel K. Vinther Wolf 
 * @brief Main Application
 * @version 0.1
 * @date 2020-09-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */

/*! \mainpage E5IOT E-2020, AU Herning, Daniel K. Vinther Wolf
 *
 * \section intro_sec Introduction
 *
 * This project is developed as part of the course Internet of Things,
 * that is held at Aarhus University Campus Herning.
 * The purpose is to make use of an IoT Device, that uses the following:
 * * a Webhook that retrives a weather report for the device to make decisions
 * * a Webhook that uses IFTTT to control Sonos API
 * * local sensors and actuators to control a Waffle Iron
 * 
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Connect the components
 * Refer to the Electrical Scehmatic and Part List
 */

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

/* Constants */
// Max retries to see if Waffle Iron is powered on
#define MAX_RETRIES 12
#define SIGOK 200
#define SIGERROR 3000
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

#pragma region Baking timer settings
// Baking and "baking watchdog" Timer setup:
// Set Baking time in Minutes and make it run only once
#define SET_BAKING_TIME (5min)
#define ONE_SHOT true

void bakingIsDone();
void stillBaking();

Timer bakingTimer(20000, bakingIsDone, ONE_SHOT);
Timer bakeWatchdogTimer(5000, stillBaking);
bool doneBaking = false;
#pragma endregion Baking timer settings

#pragma endregion init

#pragma region setup()
/**
 * @brief Runs once when the device is first turned on, but
 * 
 */
void setup()
{
    // Initialize Pinsettings (refer to pinsettings.h for pinout):
    initAngleSensor();
    initTouchSensor();
    initRelay();
    setRelay("off");

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
/**
 * @brief Runs over and over again, as quickly as it can execute
 * 
 */
void loop()
{
    digitalWrite(BLUE_LED, LOW);
    Log.info("%s Goind to sleep! Activate touch sensor to wake up",
             Time.timeStr().c_str());
    buzzer.playGoingToSleep();
    System.sleep(config);

    // Sleeps until Touch Sensor is activate
    miotyWaffles();
}
#pragma endregion loop()

#pragma region MiotyWaffles(Application)

/**
 * @brief MiotyWaffles application
 * 
 */
void miotyWaffles()
{
#pragma region mioty init

    toggleLed(BLUE_LED);
    buzzer.playWokeUp();
    Log.info("%s Touch Sensor Activated! Awake now",
             Time.timeStr().c_str());
    // Initialize RGB Sensor
    if (rgb.sensorIsConnected())
    {
        Log.info("%s RGB Sensor Initialized %d", Time.timeStr().c_str(), rgb.deviceID);
        delay(1000);
    }
    else
    {
        Log.error("%s RGB SENSOR NOT PRESENT", Time.timeStr().c_str());
        buzzer.beep(SIGERROR);
        return;
    }

    Log.info("%s Get Weather Report", Time.timeStr().c_str());
    weather.getWeatherReport();

    // Enable Waffle Iron by turning Relay On
    setRelay("on");
    buzzer.beep(SIGOK);

    // Wait for LED to be Red/Orange
    int retry = 0;
    delay(200);
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
            digitalWrite(BLUE_LED, LOW);
            setRelay("off");
            buzzer.beep(SIGERROR);
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

    // Change Track on Sonos Speaker
    if (!sc.SonosPlay("heating"))
    {
        Log.warn("%s Connection Lost: Music cannot play!",
                 Time.timeStr().c_str());
        buzzer.playError();
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
            setRelay("off");
            buzzer.beep(SIGERROR);
            return;
        }
        buzzer.beep(SIGOK);
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
        buzzer.playError();
    }
    Log.info("%s Music Should Change About Now!", Time.timeStr().c_str());

    // Wait for user to open lid:
    while (!lidIsOpen())
    {
        toggleLed(BLUE_LED);
        delay(1000);
    }
    Log.info("%s Lid was Opened!", Time.timeStr().c_str());
    buzzer.beep(SIGOK);

    // Wait for WaffleIron Lid to be closed:
    while (lidIsOpen())
    {
        toggleLed(BLUE_LED);
        delay(100);
    }
    buzzer.beep(SIGOK);
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
        buzzer.playError();
    }
    Log.info("%s Music Should Change About Now!", Time.timeStr().c_str());
#pragma endregion State baking
#pragma region Process is Done
    while (!doneBaking)
    {
        // wait for baking timer to run out
        delay(10);
    }

    setRelay("off");

    // Check that Relay turned off after a small delay
    // I.e. no LED light from waffleiron
    delay(300);
    if (!rgb.getColor() == noChange)
    {
        Log.warn("%s Tries to turn off Relay one more time!", Time.timeStr().c_str());
        setRelay("off");
        delay(1000);
        if (!rgb.getColor() == noChange)
        {
            Log.error("%s Relay did not turn off!", Time.timeStr().c_str());
            buzzer.playError(SIGERROR);
            return;
        }
    }

    Log.info("%s All Done! Have an Enjoyable Day!", Time.timeStr().c_str());
    digitalWrite(BLUE_LED, LOW);
    buzzer.beep(SIGOK);
    return;
}
#pragma endregion Process is Done
#pragma endregion MiotyWaffles(Application)

#pragma region support functions and events

/**
 * @brief Run once when the baking timer runs out
 * 
 */
void bakingIsDone()
{
    Log.info("%s Baking Timer Ran out", Time.timeStr().c_str());
    doneBaking = true;
    bakingTimer.stop();
}

/**
 * @brief runs every 5second when the baking timer runs
 * 
 */
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

/**
 * @brief Evaluates the received weather report
 * 
 * @param event miotywaffles_owm_report
 * @param data JSON schema with current weather conditions for Aarhus
 */
void recievedWeatherReport(const char *event, const char *data)
{
    weather.evaluateReport(data);
}

/**
 * @brief Read GPO pin and set the opposite state
 * 
 * @param pin GPO
 */
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