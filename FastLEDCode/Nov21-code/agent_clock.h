
#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <stdint.h>
#include <Time.h>


#define SECMIN 30 //temp variable to scale down seconds/mins for testing

class AgentClock 
{
  public:
     AgentClock(CRGBPalette16 p_sec_palette, CRGBPalette16 p_min_palette, CRGBPalette16 p_hour_palette, 
                   uint16_t p_num_leds, uint8_t p_agents_here[], TBlendType p_blending, uint8_t p_brightness);
     void drawagentclock(uint8_t agents_here[], CRGB leds[], boolean stealth, CRGBPalette16 bgpalette);
     CRGBPalette16 sec_palette;
     CRGBPalette16 min_palette;
     CRGBPalette16 hour_palette; 
    
  private:
     uint8_t num_leds;
     //these vals are scaled down for testing
     Agent sec_agents[SECMIN];
     Agent min_agents[SECMIN];
     //this one is not scaled down
     Agent hour_agents[24];
     TBlendType    blending;
     uint8_t brightness;
     void updateagents(uint8_t live, Agent agents[], uint8_t agents_here[], 
                                CRGBPalette16 palette, uint8_t max_i);
    void drawagents(CRGB leds[], Agent agents[], uint8_t max_i);
  
};
