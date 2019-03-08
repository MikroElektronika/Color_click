/*
Example for Color Click

    Date          : apr 2018.
    Author        : Katarina Perendic

Test configuration MSP :
    
    MCU              : MSP432
    Dev. Board       : Clicker 2 for MSP432
    ARM Compiler ver : v6.0.0.0

---

Description :

The application is composed of three sections :

- System Initialization - Initializes I2C module
- Application Initialization - Initializes driver init and configuration chip for measurement
- Application Task - (code snippet) - Read color and check which color is detected by the sensor
                                      If the color is detected, the detected color message is logged on the USBUART.

*/

#include "Click_Color_types.h"
#include "Click_Color_config.h"

uint8_t isColor = 0;
float colorValue;
char dataText[ 20 ];

void systemInit()
{
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_INT_PIN, _GPIO_INPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_PWM_PIN, _GPIO_OUTPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_CS_PIN, _GPIO_OUTPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_AN_PIN, _GPIO_OUTPUT );

    mikrobus_i2cInit( _MIKROBUS1, &_COLOR_I2C_CFG[0] );
    mikrobus_logInit( _MIKROBUS2, 9600 );
    mikrobus_logWrite("--- System Init --- ", _LOG_LINE);
    Delay_ms( 100 );
}
void applicationInit()
{
    color_i2cDriverInit( (T_COLOR_P)&_MIKROBUS1_GPIO, (T_COLOR_P)&_MIKROBUS1_I2C, 0x29 );
    mikrobus_logWrite("--- Color Init --- ", _LOG_LINE);
    color_writeByte(_COLOR_REG_ENABLE, _COLOR_ENABLE_RGBC_INTERRUPT |
                                         _COLOR_ENABLE_WAIT |
                                         _COLOR_ENABLE_RGBC |
                                         _COLOR_POWER_ON );

    color_writeByte(_COLOR_REG_CONTROL, _COLOR_CTRL_GAIN_x16);
    color_writeByte(_COLOR_REG_RGBC_TIME, _COLOR_RGBC_TIME_700ms);
    color_setLED(_COLOR_LED_RED_ENABLE, _COLOR_LED_GREEN_ENABLE, _COLOR_LED_BLUE_ENABLE);
    mikrobus_logWrite("--- Start measurement --- ", _LOG_LINE);
}

void applicationTask()
{
    colorValue = color_getColorValue();
    isColor = color_getColor(colorValue);

  switch(isColor)
    {
        case 1:
        {
            mikrobus_logWrite("--- Color: ORANGE ", _LOG_LINE);
            break;
        }
        case 2:
        {
            mikrobus_logWrite("--- Color: RED ", _LOG_LINE);
            break;
        }
        case 3:
        {
            mikrobus_logWrite("--- Color: PINK ", _LOG_LINE);
            break;
        }
        case 4:
        {
            mikrobus_logWrite("--- Color: PURPLE ", _LOG_LINE);
            break;
        }
        case 5:
        {
            mikrobus_logWrite("--- Color: BLUE ", _LOG_LINE);
            break;
        }
        case 6:
        {
            mikrobus_logWrite("--- Color: CYAN ", _LOG_LINE);
            break;
        }
        case 7:
        {
            mikrobus_logWrite("--- Color: GREEN ", _LOG_LINE);
            break;
        }
        case 8:
        {
            mikrobus_logWrite("--- Color: YELLOW ", _LOG_LINE);
            break;
        }
        default:
        {
            break;
        }
    }
    Delay_100ms();
}

void main()
{
    systemInit();
    applicationInit();

    while (1)
    {
            applicationTask();
    }
}
