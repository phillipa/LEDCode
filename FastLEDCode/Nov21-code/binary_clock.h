
#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <stdint.h>
#include <Time.h>



class BinaryClock 
{

  public:
    BinaryClock(CRGB p_zero, CRGB p_one, TBlendType p_blending, uint8_t p_brightness);
    void plotnumber(CRGB leds[],int num, int start_index, int num_bits, CRGB one);
    void plotnumber(CRGB leds[],int num, int start_index, int num_bits, CRGB one, CRGB zero);
    void drawbinaryclock(CRGB leds[], CRGBPalette16 bgpalette, uint8_t start_index, uint16_t num_leds);
    CRGB one;
    CRGB zero;
  private:
     TBlendType    blending;
     uint8_t brightness;  
  
};

