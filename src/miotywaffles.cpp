/***************************************************************************/ /**
 * 
 * Source File	: 	miotywaffles.cpp
 * Author		: 	Daniel K. Vinther Wolf 
 * Created		:	20200917
 * Version		:	0.3.1
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
#include "./Sensors/anglesensor.hpp"
// Actuators:
//#include "./Actuators/<>.hpp"

// Web Services:
#include "./WebServices/weather.hpp"
#include "./WebServices/sonos.hpp"

#define TIME_LIMIT 12
// Set Baking time in Minutes
#define SET_BAKING_TIME (5min)
#define ONE_SHOT true

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
int RgbColor = 0;

// LEDS
bool BlueLED = false;

// Forward Definitions
void miotyWaffles();
bool toggleBlueLED(bool BlueLED);
void goToSleepPattern();
void wakeUpPattern();
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
/***********************************************************************
 *  @brief setup() runs once when the device is first turned on.
 ***********************************************************************/
void setup()
{
    // Initialize Pinsettings (refer to pinsettings.h for pinout):
    pinMode(ANGLE_SENSOR, INPUT);
    pinMode(TOUCH_SENSOR, INPUT);
    pinMode(BLUE_LED, OUTPUT);
    BlueLED = toggleBlueLED(BlueLED);

    // System Settings, set system sleep between runs
    config.mode(SystemSleepMode::STOP)
        .gpio(TOUCH_SENSOR, RISING);

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

    Particle.subscribe("hook-response/miotywaffles_owm_report", recievedWeatherReport, MY_DEVICES);
}

/***********************************************************************
 *  @brief loop() runs over and over again, as quickly as it can execute.
 ***********************************************************************/
void loop()
{
    // Sleeps until Touch Sensor is activated
    Log.info("%s Waiting for Touch Sensor Activation!",
             Time.timeStr().c_str());
    goToSleepPattern();
    System.sleep(config);
    wakeUpPattern();
    Log.info("%s Touch Sensor Activated! Awake now",
             Time.timeStr().c_str());
    miotyWaffles();
    delay(10000);
}

void miotyWaffles()
{
    int elapsedTime = 0;

    BlueLED = toggleBlueLED(BlueLED);
    delay(20000);
    // TO-DO: Enable Waffle Iron by turning Relay On
    BlueLED = toggleBlueLED(BlueLED);
    // Initialize RGB Sensor

    if (rgb.sensorIsConnected())
    {
        Log.info("%s RGB Sensor Initialized %d", Time.timeStr().c_str(), rgb.deviceID);
        delay(1000);
    }
    else
    {
        Log.error("%s RGB SENSOR NOT PRESENT", Time.timeStr().c_str());
        exit(EXIT_FAILURE);
    }

    Log.info("%s Get Weather Report", Time.timeStr().c_str());

    delay(100);
    weather.getWeatherReport();

    // Wait for LED to be Red/Orange
    while (rgb.getColor() != Red)
    {
        ++elapsedTime;
        delay(5000);

        Log.info("%s Color is %d", Time.timeStr().c_str(), RgbColor);
        Log.info("%s Colors: Red=%d Green=%d Diff=%d", Time.timeStr().c_str(),
                 rgb.redValue, rgb.greenValue, rgb.redValue - rgb.greenValue);

        // Exit if LED is not RED/Orange within Time_limit
        if (elapsedTime >= TIME_LIMIT)
        {
            Log.error("Waffle Iron is not powered ON!");
            return;
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
        Log.warn("%s Connection Lost: Music cannot play!",
                 Time.timeStr().c_str());
        // TO-DO: Buzzer ->> Beep Beep
    }
    Log.info("%s Music Should Change About Now!",
             Time.timeStr().c_str());

    // Wait for WaffleIron LED to be Green
    elapsedTime = 0;
    while (rgb.getColor() != Green)
    {
        ++elapsedTime;
        Log.info("%s Color is %d", Time.timeStr().c_str(), RgbColor);
        Log.info("%s Colors: Red=%d Green=%d Diff=%d", Time.timeStr().c_str(),
                 rgb.redValue, rgb.greenValue, rgb.redValue - rgb.greenValue);
        delay(5000);
        BlueLED = toggleBlueLED(BlueLED);
        if (elapsedTime >= 60)
        {
            Log.info("Waffle Iron is not powered ON!");
            return;
        }
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
        Log.warn("%s Connection Lost: Music cannot play!",
                 Time.timeStr().c_str());
        // Buzzer ->> Beep Beep
    }
    Log.info("%s Music Should Change About Now!", Time.timeStr().c_str());

    // Wait for WaffleIron Lid to open:
    while (!lidIsOpen())
    {
        BlueLED = toggleBlueLED(BlueLED);
        delay(1000);
    }
    Log.info("%s Lid was Opened!", Time.timeStr().c_str());

    // Wait for WaffleIron Lid to close:
    while (lidIsOpen())
    {
        BlueLED = toggleBlueLED(BlueLED);
        delay(3000);
    }
    Log.info("%s Lid was Closed!", Time.timeStr().c_str());
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
    // Fast Paced delay to simulate Waffleiron

    // Change Track on Sonos System
    if (!sc.SonosPlay("baking"))
    {
        Log.warn("%s Connection Lost: Music cannot play!",
                 Time.timeStr().c_str());
        // Buzzer ->> Beep Beep
    }
    Log.info("%s Music Should Change About Now!", Time.timeStr().c_str());

    while (!doneBaking)
    {
        // wait for baking timer to run out
        delay(10);
    }

    // TO-DO: Turn off Relay

    // TO-DO: Check that Relay turned off (no LED light from waffleiron)

    delay(6000);
    Log.info("%s All Done! Have an Enjoyable Day!", Time.timeStr().c_str());
    return;
}

void bakingIsDone()
{
    Log.info("%s Baking Timer Ran out", Time.timeStr().c_str());
    doneBaking = true;
}

void stillBaking()
{
    Log.info("%s Still baking", Time.timeStr().c_str());
    BlueLED = toggleBlueLED(BlueLED);
}

void recievedWeatherReport(const char *event, const char *data)
{
    // Check if it is Good or Bad Weather from OpenWeatherMap JSON Response
    weather.evaluateReport(data);
}

bool toggleBlueLED(bool BlueLED)
{
    if (!BlueLED)
    {
        digitalWrite(BLUE_LED, HIGH);
        return true;
    }
    else
    {
        digitalWrite(BLUE_LED, LOW);
        return false;
    }
}

void goToSleepPattern()
{
    for (int i = 0; i < 5; i++)
    {
        for (int i = 0; i < 4; i++)
        {
            BlueLED = toggleBlueLED(BlueLED);
            delay(150);
        }
        delay(300);
    }
    if (BlueLED)
    {
        BlueLED = toggleBlueLED(BlueLED);
    }
}

void wakeUpPattern()
{
    for (int i = 0; i < 3; i++)
    {
        for (int i = 0; i < 3; i++)
        {
            BlueLED = toggleBlueLED(BlueLED);
            delay(50);
        }
        delay(200);
    }
    if (BlueLED)
    {
        BlueLED = toggleBlueLED(BlueLED);
    }
}