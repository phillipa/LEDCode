
#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <stdint.h>
#include <Time.h>
#include "agent.h"
#include "agent_clock.h"


AgentClock::AgentClock(CRGBPalette16 p_sec_palette, CRGBPalette16 p_min_palette, CRGBPalette16 p_hour_palette, 
    uint16_t p_num_leds, uint8_t p_agents_here[],TBlendType p_blending, uint8_t p_brightness)
{
  //set palettes
  sec_palette = p_sec_palette;
  min_palette = p_min_palette;
  hour_palette = p_hour_palette;

  num_leds = p_num_leds;

  //set brightness/blending
  blending=p_blending;
  brightness=p_brightness;

  //init agent positions logger.
  for(int i = 0; i < num_leds; i++)
    p_agents_here[i] = 0;
    
  uint16_t pos;
  //init the agents
  for(int i = 0; i < SECMIN; i++)
  { 
    pos = random(1, SECLEDS);
    sec_agents[i].initagent(ColorFromPalette(sec_palette, random(1,255)), pos, 2,0,SECLEDS);
    p_agents_here[pos] ++;
    pos = random(SECLEDS, SECLEDS+MINLEDS);
    min_agents[i].initagent(ColorFromPalette(min_palette, random(1,255)), pos, 4, SECLEDS,SECLEDS+MINLEDS);
    p_agents_here[pos] ++;
  }
  for(int i = 0; i < 12; i++)
  {
    pos = random(SECLEDS+MINLEDS, SECLEDS+MINLEDS+HOURLEDS);
    hour_agents[i].initagent(ColorFromPalette(hour_palette, random(1,255)), pos, 8,SECLEDS+MINLEDS, SECLEDS+MINLEDS+HOURLEDS);
    p_agents_here[pos] ++;
  }
}

void AgentClock::drawagentclock(uint8_t agents_here[], CRGB leds[], boolean stealth, CRGBPalette16 bgpalette, uint16_t start_index)
{
 /* uint8_t seconds = second() >> 1;
  uint8_t minutes = minute() >> 1;
  uint8_t hours = hour(); //don't divide hour by 2
*/
 uint8_t seconds = (second() * SECLEDS) / 60;
  uint8_t minutes = (minute() * MINLEDS) / 60 ;
  if(minute() > 0 && minutes < 1)
    minutes = 1;
  uint8_t hours = (hour()+1) >> 2; // hour is 1-12
   if(hour() > 0  && hours < 1)
    hours = 1;
  //update agent colorings and move them as required 
  updateagents(seconds, sec_agents,agents_here, sec_palette,SECMIN);
  updateagents(minutes, min_agents,agents_here, min_palette,SECMIN);
  updateagents(hours, hour_agents,agents_here, hour_palette,12);

  //draw the agents.
  drawagents(leds, sec_agents, SECMIN);
  drawagents(leds, min_agents, SECMIN);
  drawagents(leds, hour_agents, 12);

  
  //draw the background
 for (int i = 0; i < num_leds; i++)
  {
    if (agents_here[i] == 0)
    {
      if (stealth)
      {
        if(start_index >= 0 )
        {
        leds[i] = ColorFromPalette( bgpalette, start_index, brightness, blending);
        start_index += 2; 
        }
        else 
        leds[i] = ColorFromPalette( bgpalette, i, brightness, blending);
      }
      else
        leds[i] = CRGB::Black;

    }

  } 


}

void AgentClock::drawagents(CRGB leds[], Agent agents[], uint8_t max_i)
{
  for (int i = 0; i < max_i; i++)
  {
    if (agents[i].color != (CRGB)CRGB::Black)
      leds[agents[i].pos] = agents[i].color;
  }

}

//call this to fix agent colors when switching palettes
void AgentClock::recoloragents()
{
    uint8_t seconds = second() >> 1;
    uint8_t minutes = minute() >> 1;
    uint8_t hours = hour(); //don't divide hour by 2

   updateagentscolor(seconds, sec_agents, sec_palette, SECMIN);
   updateagentscolor(minutes, min_agents, min_palette, SECMIN);
   updateagentscolor(hours, hour_agents, hour_palette, 12);
}

void AgentClock::updateagentscolor(uint8_t live, Agent agents[], 
                                CRGBPalette16 palette, uint8_t max_i)
{
  for(int i = 0; i < max_i ; i++)
  {
   if (i < live)
        agents[i].color = ColorFromPalette( palette, random(1, 256), brightness, blending);
    else if (agents[i].color != (CRGB)CRGB::Black)
        agents[i].color = CRGB::Black;

  }
}

void AgentClock::updateagents(uint8_t live, Agent agents[], uint8_t agents_here[], 
                                CRGBPalette16 palette, uint8_t max_i)
{
  for(int i = 0; i < max_i ; i++)
  {
   if (i < live)
    {
      if (agents[i].color == (CRGB)CRGB::Black)
      {
        agents[i].color = ColorFromPalette( palette, random(1, 256), brightness, blending);
        agents_here[agents[i].pos]++;
      }
      agents[i].move(agents_here, num_leds);
    }
    else if (agents[i].color != (CRGB)CRGB::Black)
    {
      agents[i].color = CRGB::Black;
      agents_here[agents[i].pos]--;
    }
  }
}

