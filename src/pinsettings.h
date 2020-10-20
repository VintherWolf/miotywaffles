/***************************************************************************/ /**
 * 
 * Source File	: 	pinsettings.h
 * Author		: 	Daniel K. Vinther Wolf 
 * Created		:	20200915
 * Version		:	0.1.0
 * 
 * Description	:	This header file is used to keep track of the mapping of
 *                  pins on the associated IoT device "particle Argon".
 *                  I.e. this is a "FPGA master constraint file" equivalent
 *                  Visit argon schematic for more info:
 *                  "https://github.com/particle-iot/argon/blob/master/pdfs/
 *                   argon-v1.00-schematic.pdf"
 * 
 ******************************************************************************/

/**
 * @brief SIP-12 HEAD2 
 * 12x1 Header includes I2C, GPIO, SPI, PWM, VUSB (5V), EN, Li+ (Batt+)
 * 
 */

// Pin  |   Description

// 12       Li+     (Battery +)
//

// 11       Enable (Pull down to disable)
//

// 10       VUSB (5V from USB Host)
//

// 9        D8
//const int <DESCRIPTION> = D8;

// 8        D7 (on-board blue LED)
//const int BLUE_LED = D7;

// 7        D6
//const int <DESCRIPTION> = D6;

// 6        D5
//const int <DESCRIPTION> = D5;

// 5        D4
//const int <DESCRIPTION> = D4;

// 4        D3
const int TOUCH_SENSOR = D3;

// 3        D2
const int ANGLE_SENSOR = D2;

// 2        SCL
// RGB Sensor (internal 10k pull up)

// 1        SDA
// RGB Sensor: Slave Addr. 29 in use, (internal 10k pull up)

/**
 * @brief SIP-16 HEAD1 
 * 16x1 Header includes GPIO, PWM, UART, SPI, ADC, GND, Mode, 3V3 and Reset
 * 
 */

// 16       Not Connect

// 15       TX (UART)

// 14       RX (UART)

// 13       MISO

// 12       MOSI

// 11       SCK (SPI)

// 10       A5

// 9        A4

// 8        A3

// 7        A2

// 6        A1

// 5        A0

// 4        GND

// 3        Mode

// 2        3V3
// Used: RGB Sensor (<100mA)

// 1        Reset