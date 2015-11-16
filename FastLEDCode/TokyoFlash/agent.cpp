#include "agent.h"

#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <stdint.h>

//empty default constructor
Agent::Agent()
{
  
}

void Agent::initagent(CRGB p_color, uint16_t p_pos, uint8_t p_max_wait, 
uint16_t p_min_pos, uint16_t p_max_pos)
{
  color = p_color;
  pos = p_pos;
  min_pos = p_min_pos;
  max_pos = p_max_pos;
  //set random direction
  dir = random(1, 6); //randomize directions
  if (dir > 3)
    dir =  - 1;
  else
    dir = 1;
  
  max_wait = p_max_wait;
  waited = 0;
}

/**
 * Have the agents move based on their wait time and direction
 */
void Agent::move(uint8_t agents_here[], uint16_t num_leds)
{
  if (waited == max_wait)
  {
    agents_here[pos]--;
    pos += dir;
    if (pos < min_pos)
     { pos=max_pos-1 ;
   //  dir *= -1;
     }
     else if(pos >= max_pos)
     {
    pos = min_pos;
    // dir*=-1;
     }
    agents_here[pos]++;
 
    waited = 0;
  }
  else
    waited++;
}


