
#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <stdint.h>
#include <Time.h>
#include "binary_clock.h"

/**
 * NOTE:
 * only does negative clock (black = one) with undulating background. 
 * Still need to add function to just show regular binary time with solid 1/0 colors.
 * 
 */
BinaryClock::BinaryClock(CRGB p_zero, CRGB p_one, TBlendType p_blending, uint8_t p_brightness)
{
  zero = p_zero;
  one = p_one;
  blending = p_blending;
  brightness = p_brightness;
}

/** 
 *  Draw the binary clock with undulating background palette
 */
void BinaryClock::drawbinaryclock(CRGB leds[], CRGBPalette16 bgpalette, uint8_t start_index, uint16_t num_leds)
{
  //plot the background palette
  for ( int i = 0; i < num_leds; i++) {
    leds[i] = ColorFromPalette( bgpalette, start_index, brightness, blending);
    start_index += 3;
  }

  //plot the time negative. 
  int curr_index = 0;
  while (curr_index < num_leds)
  {
    plotnumber(leds, second(), curr_index, 6,
               CRGB::Black);
    curr_index += 6;
    plotnumber(leds, minute(), curr_index, 6, CRGB::Black);
    curr_index += 6;
    plotnumber(leds, hour(), curr_index, 4, CRGB::Black);
    curr_index += 4;
  }
}

void BinaryClock::plotnumber(CRGB leds[], int num, int start_index, int num_bits, CRGB p_one, CRGB p_zero)
{
  for (int i = num_bits - 1; i >= 0; i--)
  {
    if ((num >> i) & 0x01)
      leds[start_index + i] = p_one;
    else
    {
      leds[start_index + i] = p_zero;
    }
  }

}
//same as prev but doesn't plot 0's
void BinaryClock::plotnumber(CRGB leds[], int num, int start_index, int num_bits, CRGB p_one)
{
  for (int i = num_bits - 1; i >= 0; i--)
  {
    if ((num >> i) & 0x01)
      leds[start_index + i] = p_one;

  }
}
