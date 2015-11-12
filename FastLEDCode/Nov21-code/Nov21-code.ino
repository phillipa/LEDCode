/********************
 * Nov 21
 *
 * Playing with FastLED library
 *
 * Based off ColorPalette example from FastLED example code.
 *
 *
 */

#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <stdint.h>
#include <Time.h>
#include "agent.h"
#include "agent_clock.h"
#include "ad_palettes.h"



//other definitions
#define DATA_PIN     17
#define NUM_LEDS    114
#define BRIGHTNESS  64 //took brightness down by 32 PG
#define UPDATES_PER_SECOND 100
CRGBPalette16 currentPalette; //assign before palette color grabbing
TBlendType    currentBlending;

CRGB leds[NUM_LEDS];

/** generic agent population variables **/
#define NUM_AGENTS 30
uint8_t agents_here[NUM_LEDS];
Agent agents[NUM_AGENTS];

/** Agent Clock Variables **/
uint8_t clock_agents_here[NUM_LEDS];
// start with moody arlene + deki clock
AgentClock aclock(darkpurples, blues, greens, NUM_LEDS, clock_agents_here, currentBlending, BRIGHTNESS);
CRGBPalette16 bgpalette = adbasic;


void setup() {

  //Set up the LEDs
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );
  for (int i = 0; i < NUM_LEDS; i++) //make them all black
    leds[i] = CRGB::Red;
  FastLED.show();
  delay(2000);//show red for 2 secs so i know it rebooted.

  //set up random numbers
  randomSeed(analogRead(0));
  randomSeed(analogRead(random(0, 7)));

  currentBlending = LINEARBLEND;

  /** set up general agent population **/
  for(int i = 0; i < NUM_LEDS;i++)
    agents_here[i]=0;
  for (int i = 0; i < NUM_AGENTS; i++)
  {
   
    uint8_t pos = random(1, NUM_LEDS);
    agents[i].initagent(ColorFromPalette(adbasic, random(1,255)), pos, 0);
    agents_here[pos] ++;
  }

}

void loop()
{
  /**Example, moving an drawing agents **/
//  move_agents(agents, agents_here);
//  draw_agents(agents, agents_here);

  /** PG: I can't decide how I want to switch between the different agent clocks at this point. 
   *  but they work 
   */
  aclock.drawagentclock(clock_agents_here, leds, 1, bgpalette);

  if(minute()%2 ==0)
    switchclock();
    
  FastLED.show(); 
  FastLED.delay(1000 / UPDATES_PER_SECOND);

}

void switchclock()
{
  if(bgpalette == adbasic) //go from gothy to Arlene clock
  {
    aclock.sec_palette = pinks;
    aclock.min_palette = yellows;
    aclock.hour_palette = whites;
    bgpalette = abasic;
  }
  else //arlene to gothy clock
    {
    aclock.sec_palette = darkpurples;
    aclock.min_palette = blues;
    aclock.hour_palette = greens;
    bgpalette = adbasic;
  }
  
}
void move_agents(Agent p_agents[], uint8_t p_agents_here[])
{
  for(int i = 0; i < NUM_AGENTS; i++)
  {
    p_agents[i].move(p_agents_here, NUM_LEDS);
  }

}

void draw_agents(Agent p_agents[], uint8_t p_agents_here[])
{
  for(int i = 0; i < NUM_AGENTS; i++)
  {
    leds[p_agents[i].pos] = p_agents[i].color;
  }
   for (int i = 0; i < NUM_LEDS; i++)
  {
    if (p_agents_here[i] == 0)
      leds[i] = CRGB::Black;
  }
}



/******************* START BINARY CLOCK FUNCTIONS *******************/
/**
 * Plots time using binary numbers repeating over the strip
 * Currently reads time from start of code.
 * TODO: get real time data.
 * TODO: see if this is too dislexic
 * shows second-minute-hour (L->R)
 * MSB is on RHS vs. LHS. Debate/change later.
 */
void plotBinTime(uint8_t palette_index)
{
  int curr_index = 0;

  while (curr_index < NUM_LEDS)
  {
    plotNumber(second(), curr_index, 6,
               ColorFromPalette( purplegreen, palette_index, BRIGHTNESS, currentBlending),
               ColorFromPalette( greenpurple, palette_index, BRIGHTNESS, currentBlending));
    curr_index += 6;

    plotNumber(minute(), curr_index, 6, CRGB::Lime, CRGB::BlueViolet);
    curr_index += 6;
    plotNumber(hour(), curr_index, 4, CRGB::DarkViolet, CRGB::Chartreuse);
    curr_index += 4;
  }
}
//plot the time with 'one' values as black and leave other values alone
void plotNegBinTime()
{
  int curr_index = 0;

  while (curr_index < NUM_LEDS)
  {
    plotNumber(second(), curr_index, 6,
               CRGB::Black);
    curr_index += 6;

    plotNumber(minute(), curr_index, 6, CRGB::Black);
    curr_index += 6;
    plotNumber(hour(), curr_index, 4, CRGB::Black);
    curr_index += 4;
  }
}
void plotNumber(int num, int start_index, int num_bits, CRGB one, CRGB zero)
{
  for (int i = num_bits - 1; i >= 0; i--)
  {
    if ((num >> i) & 0x01)
      leds[start_index + i] = one;
    else
    {

      leds[start_index + i] = zero;
    }
  }

}
//same as prev but doesn't plot 0's
void plotNumber(int num, int start_index, int num_bits, CRGB one)
{
  for (int i = num_bits - 1; i >= 0; i--)
  {
    if ((num >> i) & 0x01)
      leds[start_index + i] = one;

  }


}
/******************* END BINARY CLOCK FUNCTIONS *******************/

/** Plot time as seconds,minutes,hours.
 *  fractional divided by 2 (ie., renormalize seconds, minutes by 30 and hours by 12 instead of 24)
 *  to fit on the strip...
 */
void plotFracTime()
{
  int curr_index = 0;
  while (curr_index < (NUM_LEDS + 72)) //allow partial clock as long as whole clock isn't outside of strip
  {
    uint8_t seconds = second() >> 1;
    uint8_t minutes = minute() >> 1;
    uint8_t hours = hour() >> 1;
    plotXofY(curr_index + 0, seconds, 30, CRGB::Blue, CRGB::White);
    plotXofY(curr_index + 30, minutes, 30, CRGB::Red, CRGB::Black);
    plotXofY(curr_index + 60, hours, 12, CRGB::Purple, CRGB::Yellow);
    curr_index += 72;

  }
}

void plotXofY(int start_index, int num_on, int num_bits, CRGB one, CRGB zero)
{
  for (int i = 0; i < num_on; i++)
    leds[start_index + i] = one;

  if (num_on < num_bits)
  {
    for (int i = num_on; i < num_bits; i++)
      leds[start_index + i] = zero;
  }
}
void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = BRIGHTNESS;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;

  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
    if ( secondHand ==  0)  {
      currentPalette = adbasic;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 5)  {
      currentPalette = purplegreen;
      currentBlending = NOBLEND;
    }
    if ( secondHand == 10)  {
      currentPalette = greenpurple;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 15)  {
      currentPalette = adbasic;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 30)  {
      currentPalette = adbasic;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 45)  {
      currentPalette = greens;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 50)  {
      currentPalette = pinks;
      currentBlending = NOBLEND;
    }
    //  if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
  }
}








/****
 * Basic, initialize an agent that just has a color
 */
/*void init_agent(struct agent_s* toinit, CRGB color1, int16_t posi, uint8_t dir, uint8_t maxw, void *fptr)
{
  toinit->color1 = color1;
  toinit->pos = posi;
  agents_here[posi]++;
  toinit->dir = dir;
  toinit->waited = 0;
  toinit->max_wait = maxw;
  toinit->span = 1;
  toinit->agent_type = AGENT_CLASSIC;


}

/*
void reverse_agent_dir(struct agent_s* toset)
{
  toset->dir = -toset->dir;
}

void set_agent_dir(struct agent_s* toset, int8_t dir)
{
  toset->dir = dir;
}

void set_agent_type(struct agent_s* toset, uint8_t atype)
{
  toset->agent_type = atype;
}

void set_agent_span(struct agent_s* toset, uint8_t span)
{
  toset->span = span;
}

void set_agent_pos(struct agent_s* toset, int16_t pos)
{
  toset->pos = pos;
}

*/


/*void move_agents()
{
  for (int i = 0; i < NUM_AGENTS; i++)
    move_agent(&agents[i]);

}

void move_agent(struct agent_s* toupdate)
{
  if (toupdate->waited == toupdate->max_wait)
  {
    agents_here[toupdate->pos]--;
    toupdate->pos += toupdate->dir;
    while (toupdate->pos < 0)
      toupdate->pos += NUM_LEDS;
    toupdate->pos %= NUM_LEDS;
    agents_here[toupdate->pos]++;
    /* collision detection. may or may not work */

    /*for (int i = 0; i < NUM_AGENTS; i++)
    {

      if (agents[i].pos == toupdate->pos && (sameDirection(&agents[i], toupdate) ))
      {
        toupdate->dir *= -1;
        agents_here[toupdate->pos]--;
        toupdate->pos = (toupdate->pos + 1) % NUM_LEDS;
        agents_here[toupdate->pos]++;
      }
    }
    */
  /*  toupdate->waited = 0;
  }
  else
    toupdate->waited++;
}

boolean sameDirection(struct agent_s* a, struct agent_s* b)
{
  return ~(0x80 & (a->dir ^ b->dir));
}
*/








