/*
    __color_driver.c

-----------------------------------------------------------------------------

  This file is part of mikroSDK.

  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

----------------------------------------------------------------------------- */

#include "__color_driver.h"
#include "__color_hal.c"

/* ------------------------------------------------------------------- MACROS */
/* Register */
const uint8_t _COLOR_REG_ENABLE          = 0x00;
const uint8_t _COLOR_REG_RGBC_TIME       = 0x01;
const uint8_t _COLOR_REG_WAIT_TIME       = 0x02;
const uint8_t _COLOR_REG_AILTL           = 0x03;
const uint8_t _COLOR_REG_AILTH           = 0x04;
const uint8_t _COLOR_REG_AIHTL           = 0x05;
const uint8_t _COLOR_REG_AIHTH           = 0x06;
const uint8_t _COLOR_REG_PERS            = 0x0C;
const uint8_t _COLOR_REG_CONFIGURATION   = 0x0D;
const uint8_t _COLOR_REG_CONTROL         = 0x0F;
const uint8_t _COLOR_REG_DEVICE_ID       = 0x12;
const uint8_t _COLOR_REG_DEVICE_STATUS   = 0x13;
const uint8_t _COLOR_REG_CLEAR_DATA_LOW  = 0x14;
const uint8_t _COLOR_REG_CLEAR_DATA_HIGH = 0x15;
const uint8_t _COLOR_REG_RED_DATA_LOW    = 0x16;
const uint8_t _COLOR_REG_RED_DATA_HIGH   = 0x17;
const uint8_t _COLOR_REG_GREEN_DATA_LOW  = 0x18;
const uint8_t _COLOR_REG_GREEN_DATA_HIGH = 0x19;
const uint8_t _COLOR_REG_BLUE_DATA_LOW   = 0x1A;
const uint8_t _COLOR_REG_BLUE_DATA_HIGH  = 0x1B;

/* Command Register */
const uint8_t _COLOR_CMD_SELECT = 0x80;
const uint8_t _COLOR_CMD_REPEATED_BYTE_PROTOCOL = 0x00;
const uint8_t _COLOR_CMD_AUTO_INCREMENT_PROTOCOL = 0x20;

/* ENABLE Register */
const uint8_t _COLOR_ENABLE_RGBC_INTERRUPT = 0x10;
const uint8_t _COLOR_ENABLE_WAIT           = 0x08;
const uint8_t _COLOR_ENABLE_RGBC           = 0x02;
const uint8_t _COLOR_POWER_ON              = 0x01;

/* RGBC Timing Register */
const uint8_t _COLOR_RGBC_TIME_2_4ms = 0xFF;
const uint8_t _COLOR_RGBC_TIME_24ms  = 0xF6;
const uint8_t _COLOR_RGBC_TIME_101ms = 0xD5;
const uint8_t _COLOR_RGBC_TIME_154ms = 0xC0;
const uint8_t _COLOR_RGBC_TIME_700ms = 0x00;

/* Wait Time Register */
const uint8_t _COLOR_WAIT_TIME_W0_2_4ms    = 0xFF;
const uint8_t _COLOR_WAIT_TIME_W0_204ms    = 0xAB;
const uint8_t _COLOR_WAIT_TIME_W0_614ms    = 0xFF;
const uint8_t _COLOR_WAIT_TIME_W1_0_029sec = 0xFF;
const uint8_t _COLOR_WAIT_TIME_W1_2_45sec  = 0xAB;
const uint8_t _COLOR_WAIT_TIME_W1_7_4sec   = 0xFF;

/* Persistence Register */
const uint8_t _COLOR_PERS_EVERY_RGBC  = 0x00;
const uint8_t _COLOR_PERS_CLEAR_CH_1  = 0x01;
const uint8_t _COLOR_PERS_CLEAR_CH_2  = 0x02;
const uint8_t _COLOR_PERS_CLEAR_CH_3  = 0x03;
const uint8_t _COLOR_PERS_CLEAR_CH_5  = 0x04;
const uint8_t _COLOR_PERS_CLEAR_CH_10 = 0x05;
const uint8_t _COLOR_PERS_CLEAR_CH_15 = 0x06;
const uint8_t _COLOR_PERS_CLEAR_CH_20 = 0x07;
const uint8_t _COLOR_PERS_CLEAR_CH_25 = 0x08;
const uint8_t _COLOR_PERS_CLEAR_CH_30 = 0x09;
const uint8_t _COLOR_PERS_CLEAR_CH_35 = 0x0A;
const uint8_t _COLOR_PERS_CLEAR_CH_40 = 0x0B;
const uint8_t _COLOR_PERS_CLEAR_CH_45 = 0x0C;
const uint8_t _COLOR_PERS_CLEAR_CH_50 = 0x0D;
const uint8_t _COLOR_PERS_CLEAR_CH_55 = 0x0E;
const uint8_t _COLOR_PERS_CLEAR_CH_60 = 0x0F;

/* Configuration Register */
const uint8_t _COLOR_CFG_WAIT_LONG = 0x01;

/* Control Register*/
const uint8_t _COLOR_CTRL_GAIN_x1  = 0x00;
const uint8_t _COLOR_CTRL_GAIN_x4  = 0x01;
const uint8_t _COLOR_CTRL_GAIN_x16 = 0x02;
const uint8_t _COLOR_CTRL_GAIN_x60 = 0x03;

/* Color Register*/
const uint8_t _COLOR_COLOR_DATA_CLEAR = 0x14;
const uint8_t _COLOR_COLOR_DATA_RED   = 0x16;
const uint8_t _COLOR_COLOR_DATA_GREEN = 0x18;
const uint8_t _COLOR_COLOR_DATA_BLUE  = 0x1A;

/* LED */
const uint8_t _COLOR_LED_RED_ENABLE    = 0x01;
const uint8_t _COLOR_LED_GREEN_ENABLE  = 0x01;
const uint8_t _COLOR_LED_BLUE_ENABLE   = 0x01;
const uint8_t _COLOR_LED_RED_DISABLE   = 0x00;
const uint8_t _COLOR_LED_GREEN_DISABLE = 0x00;
const uint8_t _COLOR_LED_BLUE_DISABLE  = 0x00;

/* Color FLAG */
static uint8_t _ORANGE_COLOR_FLAG = 1;
static uint8_t _RED_COLOR_FLAG    = 2;
static uint8_t _PINK_COLOR_FLAG   = 3;
static uint8_t _PURPLE_COLOR_FLAG = 4;
static uint8_t _BLUE_COLOR_FLAG   = 5;
static uint8_t _CYAN_COLOR_FLAG   = 6;
static uint8_t _GREEN_COLOR_FLAG  = 7;
static uint8_t _YELLOW_COLOR_FLAG = 8;
static uint8_t _NON_COLOR_FLAG    = 0;


/* ---------------------------------------------------------------- VARIABLES */

#ifdef   __COLOR_DRV_I2C__
static uint8_t _slaveAddress;
#endif

/* -------------------------------------------- PRIVATE FUNCTION DECLARATIONS */

static float _maxColor( float color_A, float color_B );
static float _minColor( float color_A, float color_B );
static float _RGB_To_HSL(float red, float green, float blue) ;

/* --------------------------------------------- PRIVATE FUNCTION DEFINITIONS */

static float _maxColor( float color_A, float color_B )
{
    if(color_A > color_B)
    {
        return color_A;
    }
    else
    {
        return color_B;
    }
}

static float _minColor( float color_A, float color_B )
{
    if(color_A < color_B)
    {
        return color_A;
    }
    else
    {
        return color_B;
    }
}

static float _RGB_To_HSL(float red, float green, float blue)
{
    float fmax;
    float fmin;
    float Hue_Value;
    float Saturation_Value;
    float Luminance_Value;

    fmax = _maxColor(_maxColor(red, green), blue);
    fmin = _minColor(_minColor(red, green), blue);

    Luminance_Value = fmax;

    if (fmax > 0)
    {
        Saturation_Value = (fmax - fmin) / fmax;
    }
    else
    {
        Saturation_Value = 0;
    }

    if (Saturation_Value == 0)
    {
        Hue_Value = 0;
    }
    else
    {
        if (fmax == red)
        {
             Hue_Value = (green - blue) / (fmax - fmin);
        }
        else if (fmax == green)
        {
             Hue_Value = 2 + (blue - red) / (fmax - fmin);
        }
        else
        {
             Hue_Value = 4 + (red - green) / (fmax - fmin);
        }
        Hue_Value = Hue_Value / 6;

        if (Hue_Value < 0)
        {
             Hue_Value = Hue_Value + 1;
        }
    }
    return Hue_Value;
}

/* --------------------------------------------------------- PUBLIC FUNCTIONS */

#ifdef   __COLOR_DRV_SPI__

void color_spiDriverInit(T_COLOR_P gpioObj, T_COLOR_P spiObj)
{
    hal_spiMap( (T_HAL_P)spiObj );
    hal_gpioMap( (T_HAL_P)gpioObj );
}

#endif
#ifdef   __COLOR_DRV_I2C__

void color_i2cDriverInit(T_COLOR_P gpioObj, T_COLOR_P i2cObj, uint8_t slave)
{
    _slaveAddress = slave;
    hal_i2cMap( (T_HAL_P)i2cObj );
    hal_gpioMap( (T_HAL_P)gpioObj );
}

#endif
#ifdef   __COLOR_DRV_UART__

void color_uartDriverInit(T_COLOR_P gpioObj, T_COLOR_P uartObj)
{
    hal_uartMap( (T_HAL_P)uartObj );
    hal_gpioMap( (T_HAL_P)gpioObj );
}

#endif
/* ----------------------------------------------------------- IMPLEMENTATION */

void color_writeByte(uint8_t reg, uint8_t _data)
{
    uint8_t writeReg[ 2 ];

    writeReg[ 0 ] = reg | _COLOR_CMD_SELECT;
    writeReg[ 1 ] = _data;

    hal_i2cStart();
    hal_i2cWrite(_slaveAddress, writeReg, 2, END_MODE_STOP);
}

uint16_t color_readData(uint8_t reg)
{
    uint8_t writeReg[ 1 ];
    uint8_t readReg[ 2 ];
    volatile uint16_t readData;

    writeReg[ 0 ] = reg | _COLOR_CMD_SELECT;

    hal_i2cStart();
    hal_i2cWrite(_slaveAddress, writeReg, 1, END_MODE_RESTART);
    hal_i2cRead(_slaveAddress, readReg, 2, END_MODE_STOP);

    readData = readReg[ 1 ];
    readData = readData << 8;
    readData = readData | readReg[ 0 ];

    return readData;
}

float color_readColorRatio(uint8_t color)
{
    uint16_t colorData;
    uint16_t clearData;
    float colorRatio;

    colorData = color_readData(color);
    clearData = color_readData(_COLOR_COLOR_DATA_CLEAR);

    colorRatio = ((float)colorData / (float)clearData);

    return colorRatio;
}

float color_getColorValue()
{
    float Red_Ratio;
    float Green_Ratio;
    float Blue_Ratio;

    float ColorData;
    float sumColor;
    uint8_t counter;

    for (counter = 0; counter < 16; counter++)
    {
        Red_Ratio = color_readColorRatio(_COLOR_COLOR_DATA_RED);
        Green_Ratio = color_readColorRatio(_COLOR_COLOR_DATA_GREEN);
        Blue_Ratio = color_readColorRatio(_COLOR_COLOR_DATA_BLUE);

        ColorData = _RGB_To_HSL(Red_Ratio, Green_Ratio, Blue_Ratio);
        sumColor = sumColor + ColorData;
    }
    sumColor = sumColor / 16.0;

    return sumColor;
}

uint8_t color_getColor(float color_value)
{
     if((color_value >= 0.992) && (color_value <= 0.999))
     {
         return _ORANGE_COLOR_FLAG;
     }
     else if ((color_value >= 0.9750) && (color_value <= 0.9919))
     {
         return _RED_COLOR_FLAG;
     }
     else if ((color_value >= 0.920) && (color_value <= 0.9749))
     {
         return _PINK_COLOR_FLAG;
     }
     else if ((color_value >= 0.6201) && (color_value <= 0.919))
     {
         return _PURPLE_COLOR_FLAG;
     }
     else if ((color_value >= 0.521) && (color_value <= 0.6200))
     {
         return _BLUE_COLOR_FLAG;
     }
     else if ((color_value >= 0.470) && (color_value < 0.520))
     {
         return _CYAN_COLOR_FLAG;
     }
     else if ((color_value >= 0.210) && (color_value <= 0.469))
     {
         return _GREEN_COLOR_FLAG;
     }
     else if ((color_value >= 0.0650) && (color_value <= 0.180))
     {
         return _YELLOW_COLOR_FLAG;
     }

     return _NON_COLOR_FLAG;
}

uint8_t color_getInterrupt()
{
    return hal_gpio_intGet();
}

void color_setLED(uint8_t red, uint8_t green, uint8_t blue)
{
    hal_gpio_anSet(red);
    hal_gpio_csSet(green);
    hal_gpio_pwmSet(blue);
}





/* -------------------------------------------------------------------------- */
/*
  __color_driver.c

  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
   This product includes software developed by the MikroElektonika.

4. Neither the name of the MikroElektonika nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY MIKROELEKTRONIKA ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MIKROELEKTRONIKA BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------------- */