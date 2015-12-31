#include "fatagent.h"

#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <stdint.h>

#define C_BLACK CRGB::Black

//empty default constructor
FatAgent::FatAgent()
{
  
}

void FatAgent::initagent(CRGB p_color, uint16_t p_pos, uint8_t p_max_wait)
{
  color = p_color;
  pos = p_pos;
  

  //set random direction
  dir = random(1, 7); //randomize directions
  if (dir > 3)
    dir =  - 1;
  else
    dir = 1;
    
  pal = CRGBPalette16( p_color, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK );
  max_wait = p_max_wait;
  waited = 0;
  span = 8;
}

void FatAgent::setspan(uint8_t new_span)
{
   if(!span) return;  // Span of 0 is nonsensical
   span = new_span;
}

void FatAgent::draw(CRGB * LED_array, uint16_t num_leds)
{
   // Start at agent position, proceed in the direction opposite dir until span has been reached.
   uint16_t min_pos, max_pos, cur_pos; 
   uint8_t pal_index;
   
   if(!LED_array) return;  // Fear the null pointer
   if(num_leds < pos) return; // Paranoia

   if(dir>0)
   {
       max_pos = pos;
   }
   else
   {
       max_pos = pos + span - 1;
   }
   min_pos = max_pos - span + 1;  // +1 for fencepost

   if(min_pos > num_leds) min_pos = 0;  // it will wrap, fix to 0
   if(max_pos > num_leds) max_pos = num_leds; // Stay in bounds

   pal_index = 0;
   if(dir>0)
   {   
       for(cur_pos = max_pos;cur_pos>min_pos;cur_pos--)
       {
          LED_array[cur_pos] += ColorFromPalette(pal, pal_index);
          pal_index+=3;
       }
   }
   else
   {
       for(cur_pos = min_pos;cur_pos<max_pos;cur_pos++)
       {
          LED_array[cur_pos] += ColorFromPalette(pal, pal_index);
          pal_index+=3;
       }
   }   
   
}

/**
 * Have the agents move based on their wait time and direction
 */
void FatAgent::move(uint8_t agents_here[], uint16_t num_leds)
{
  if (waited == max_wait)
  {
    if(agents_here != NULL) agents_here[pos]--;
    pos += dir;
    while (pos < 0)
      pos += num_leds;
    pos %= num_leds;
    if(agents_here != NULL) agents_here[pos]++;
 
    waited = 0;
  }
  else
    waited++;
}



