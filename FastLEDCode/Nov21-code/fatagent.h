

#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <stdint.h>

class FatAgent 
{
  public:
     FatAgent();
     void initagent(CRGB p_color, uint16_t p_pos, uint8_t p_max_wait);
     void move(uint8_t agents_here[], uint16_t num_leds);
     void setspan(uint8_t new_span);
     void draw(CRGB * LED_array, uint16_t num_leds);
     CRGBPalette16 pal;
     CRGB color; //color of agent
     int16_t pos; // position of agent
     int8_t dir; // Direction (signed)
     uint8_t span; // Span of agent (unsigned, always lags position
     uint16_t max_wait; // how long this agent waits between moves
  private:
     uint16_t waited; // State counter for wait
     
  

  
};

