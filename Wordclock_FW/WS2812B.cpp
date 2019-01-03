#include <esp_log.h>
#include <driver/rmt.h>
#include <driver/gpio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdexcept>

#include "sdkconfig.h"
#include "WS2812B.h"

static char tag[] = "WS2812";

/**
 * A NeoPixel is defined by 3 bytes ... red, green and blue.
 * Each byte is composed of 8 bits ... therefore a NeoPixel is 24 bits of data.
 * At the underlying level, 1 bit of NeoPixel data is one item (two levels)
 * This means that the number of items we need is:
 *
 * #pixels * 24
 *
 */

/**
 * Set two levels of RMT output to the Neopixel value for a "1".
 * This is:
 * a logic 1 for 0.7us
 * a logic 0 for 0.6us
 */
static void setItem1(rmt_item32_t *pItem)
{
    assert(pItem != nullptr);
    pItem->level0    = 1;
    pItem->duration0 = 10;
    pItem->level1    = 0;
    pItem->duration1 = 6;
}

/**
 * Set two levels of RMT output to the Neopixel value for a "0".
 * This is:
 * a logic 1 for 0.35us
 * a logic 0 for 0.8us
 */
static void setItem0(rmt_item32_t *pItem)
{
    assert(pItem != nullptr);
    pItem->level0    = 1;
    pItem->duration0 = 4;
    pItem->level1    = 0;
    pItem->duration1 = 8;
}


/**
 * Add an RMT terminator into the RMT data.
 */
static void setTerminator(rmt_item32_t *pItem)
{
    assert(pItem != nullptr);
    pItem->level0    = 0;
    pItem->duration0 = 0;
    pItem->level1    = 0;
    pItem->duration1 = 0;
}

/*
 * Internal function not exposed.  Get the pixel channel color from the channel
 * type which should be one of 'R', 'G' or 'B'.
 */
static uint8_t getChannelValueByType(char type, pixel_t pixel)
{
    switch(type)
    {
        case 'r':
        case 'R':
              return pixel.red;
        case 'b':
        case 'B':
              return pixel.blue;
        case 'g':
        case 'G':
              return pixel.green;
    }
    
    ESP_LOGW(tag, "Unknown color channel 0x%2x", type);
    return 0;
}



/**
 * @brief Construct a wrapper for the pixels.
 *
 * In order to drive the NeoPixels we need to supply some basic information.  This
 * includes the GPIO pin that is connected to the data-in (DIN) of the devices.
 * Since we also want to be able to drive a string of pixels, we need to tell the class
 * how many pixels are present in the string.
 *

 * @param [in] gpioNum The GPIO pin used to drive the data.
 * @param [in] pixelCount The number of pixels in the strand.
 * @param [in] channel The RMT channel to use.  Defaults to RMT_CHANNEL_0.
 */
WS2812::WS2812(gpio_num_t dinPin, uint16_t pixelCount, int channel)
{
    this->pixelCount = pixelCount;
    this->channel    = (rmt_channel_t)channel;

// The number of items is number of pixels * 24 bits per pixel + the terminator.
// Remember that an item is TWO RMT output bits ... for NeoPixels this is correct because
// on Neopixel bit is TWO bits of output ... the high value and the low value

    this->items      = new rmt_item32_t[pixelCount * 24 + 1];
    this->pixels     = new pixel_t[pixelCount];
    this->colorOrder = (char *)"GRB";
    clear();

    rmt_config_t config;
    config.rmt_mode                  = RMT_MODE_TX;
    config.channel                   = this->channel;
    config.gpio_num                  = dinPin;
    config.mem_block_num             = 8-this->channel;
    config.clk_div                   = 8;
    config.tx_config.loop_en         = 0;
    config.tx_config.carrier_en      = 0;
    config.tx_config.idle_output_en  = 1;
    config.tx_config.idle_level      = (rmt_idle_level_t)0;
    config.tx_config.carrier_freq_hz = 10000;
    config.tx_config.carrier_level   = (rmt_carrier_level_t)1;
    config.tx_config.carrier_duty_percent = 50;

    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(this->channel, 0, 0));
}


/**
 * @brief Show the current Neopixel data.
 *
 * Drive the LEDs with the values that were previously set.
 */
void WS2812::show()
{
    auto pCurrentItem = this->items;
    
    for (auto i=0; i<this->pixelCount; i++)
    {
        uint32_t currentPixel = (getChannelValueByType(this->colorOrder[0], this->pixels[i]) << 16) | (getChannelValueByType(this->colorOrder[1], this->pixels[i]) << 8) | (getChannelValueByType(this->colorOrder[2], this->pixels[i]));
        ESP_LOGD(tag, "Pixel value: %x", currentPixel);
        for (int j=23; j>=0; j--)
        {
              // We have 24 bits of data representing the red, green amd blue channels. The value of the
              // 24 bits to output is in the variable current_pixel.  We now need to stream this value
              // through RMT in most significant bit first.  To do this, we iterate through each of the 24
              // bits from MSB to LSB.
              if (currentPixel & (1<<j))
              {
                    setItem1(pCurrentItem);
              }
              else
              {
                    setItem0(pCurrentItem);
              }
              pCurrentItem++;
        }
    }
    setTerminator(pCurrentItem); // Write the RMT terminator.
    // Show the pixels.
    ESP_ERROR_CHECK(rmt_write_items(this->channel, this->items, this->pixelCount*24, 1 /* wait till done */));
}


/**
 * @brief Set the color order of data sent to the LEDs.
 *
 * Data is sent to the WS2812s in a serial fashion.  There are 8 bits of data for each of the three
 * channel colors (red, green and blue).  The WS2812 LEDs typically expect the data to arrive in the
 * order of "green" then "red" then "blue".  However, this has been found to vary between some
 * models and manufacturers.  What this means is that some want "red", "green", "blue" and still others
 * have their own orders.  This function can be called to override the default ordering of "GRB".
 * We can specify
 * an alternate order by supply an alternate three character string made up of 'R', 'G' and 'B'
 * for example "RGB".
 */
void WS2812::setColorOrder(char *colorOrder)
{
    if (colorOrder != nullptr && strlen(colorOrder) == 3)
    {
        this->colorOrder = colorOrder;
    }
}



uint32_t WS2812::Color(uint8_t red, uint8_t green, uint8_t blue)
{
    return ((uint32_t)red << 16) | ((uint32_t)green <<  8) | blue;
}

/**
 * @brief Set the given pixel to the specified color.
 *
 * The LEDs are not actually updated until a call to show().
 *
 * @param [in] index The pixel that is to have its color set.
 * @param [in] red The amount of red in the pixel.
 * @param [in] green The amount of green in the pixel.
 * @param [in] blue The amount of blue in the pixel.
 */
void WS2812::setPixel(uint16_t index, uint8_t red, uint8_t green, uint8_t blue)
{
    assert(index < pixelCount);

    this->pixels[index].red   = red;
    this->pixels[index].green = green;
    this->pixels[index].blue  = blue;
}

void WS2812::setPixel(uint16_t index, uint32_t color)
{
    assert(index < pixelCount);
    if(DEBUG_WS2812B == 1)
    {
        Serial.println("WS2812B.cpp Farbanteile");
        Serial.print(uint8_t(color>>16));
        Serial.print(uint8_t(color>>8));
        Serial.print(uint8_t(color));
    }
    
    this->pixels[index].red   = uint8_t(color>>16);
    this->pixels[index].green = uint8_t(color>>8);
    this->pixels[index].blue  = uint8_t(color);
}

void WS2812::setPixel(uint16_t index)
{
    Serial.println("WS2812B.cpp Aufruf setPixel");
    assert(index < pixelCount);
    
    Serial.println("WS2812B.cpp Pixel alle blau");
    this->pixels[index].red   = 0;
    this->pixels[index].green = 0;
    this->pixels[index].blue  = 255;
}

/**
 * @brief Set all pixel to the specific color
 *
 * The LEDs are not actually updated until a call to show().
 *
 * @param [in] red The amount of red in the pixel.
 * @param [in] green The amount of green in the pixel.
 * @param [in] blue The amount of blue in the pixel.
 */
void WS2812::setAllPixels(uint8_t red, uint8_t green, uint8_t blue)
{
    for (auto i=0; i<this->pixelCount; i++)
    {
        this->pixels[i].red   = red;
        this->pixels[i].green = green;
        this->pixels[i].blue  = blue;
    }
}

void WS2812::setAllPixels(uint32_t color)
{
    for (auto i=0; i<this->pixelCount; i++)
    {
        this->pixels[i].red   = uint8_t(color>>16);
        this->pixels[i].green = uint8_t(color>>8);
        this->pixels[i].blue  = uint8_t(color);
    }
}

/**
 * @brief Set the starting pattern of the LEDs on wordclock start
 *
 * The LEDs show a specific behaviour on the clock
 *
 * @param [in] The starting pattern version.
 *  0: all LEDs off
 *  1: 
 *  9: all LEDs in red
 */
void WS2812::startPattern(uint8_t version)
{
    if(DEBUG_WS2812B == 1)
    {
        Serial.print("WS2812B.cpp Startpattern für WS2812B ");
        Serial.println(version);
    }
    
    switch(version)
    {
        if(DEBUG_WS2812B == 1)
        {
            Serial.print("WS2812B.cpp Startmuster ");
        }
        case 0: //alle LEDs aus
            if(DEBUG_WS2812B == 1)
            {
                Serial.print(version);
                Serial.print(" - alle LEDs ausschalten");
            }
            this->clear();
            this->show();
            break;
        case 1:
            if(DEBUG_WS2812B == 1)
            {
                Serial.print(version);
                Serial.println(" - tbd");
            }
            break;
        case 9:
            if(DEBUG_WS2812B == 1)
            {
                Serial.print(version);
                Serial.println(" - alle LEDs rot leuchten lassen");
            }
            this->setAllPixels(255,0,0);//voll ROT
            this->show();
        default:
            break;
    }
    
}

/**
 * @brief Clear all the pixel colors.
 *
 * This sets all the pixels to off which is no brightness for all of the color channels.
 * The LEDs are not actually updated until a call to show().
 */
void WS2812::clear()
{
    for (auto i=0; i<this->pixelCount; i++)
    {
        this->pixels[i].red   = 0;
        this->pixels[i].green = 0;
        this->pixels[i].blue  = 0;
    }
}

/**
 * @brief 
 *
 * 
 */
uint16_t WS2812::getPixelCount()
{
    return this->pixelCount;
}

/**
 * @brief Class instance destructor.
 */
WS2812::~WS2812()
{
    delete this->items;
    delete this->pixels;
}
