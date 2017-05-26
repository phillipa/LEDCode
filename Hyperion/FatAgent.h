

#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <stdint.h>

#define CTYPE_CRGB 0

#define _up 0
#define _down 1
#define _left 2
#define _right 3

class FatAgent 
{
  public:
     FatAgent();
     void initagent(CRGB p_color, uint16_t p_x,uint16_t p_y, uint8_t p_max_wait);
     void move(uint8_t agents_here[], int row_width, int num_rows, uint16_t num_leds);
     void setspan(uint8_t new_span);
     void setcolor(CRGB p_color);

     void draw(CRGB * LED_array, int row_width, int num_rows, uint16_t num_leds);
     CRGBPalette16 pal;
     CRGB color; //color of agent
     int16_t x; // position of agent
     int16_t y; // position of agent

     int8_t dir; // Direction (signed)
     uint8_t span; // Span of agent (unsigned, always lags position
     uint16_t max_wait; // how long this agent waits between moves
  private:
     uint16_t waited; // State counter for wait
     
  

  
};

