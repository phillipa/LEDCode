#include "agent.h"

#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <stdint.h>

//empty default constructor
Agent::Agent()
{
  
}

void Agent::initagent(CRGB p_color, uint16_t p_pos, uint8_t p_max_wait)
{
  color = p_color;
  pos = p_pos;

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
    while (pos < 0)
      pos += num_leds;
    pos %= num_leds;
    agents_here[pos]++;
 
    waited = 0;
  }
  else
    waited++;
}


