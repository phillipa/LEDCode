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
#include "binary_clock.h"
#include "spark.h"



//other definitions
#define DATA_PIN     17
#define NUM_LEDS    478
#define BRIGHTNESS  128 //took brightness down by 32 PG
#define UPDATES_PER_SECOND 100
#define BLENDING LINEARBLEND
CRGBPalette16 currentPalette; //assign before palette color grabbing
TBlendType    currentBlending;

CRGB leds[NUM_LEDS];

/** generic agent population variables **/
#define NUM_AGENTS 60
uint8_t agents_here[NUM_LEDS];
Agent agents[NUM_AGENTS];

/** generic spark population variables **/
#define NUM_SPARKS 80
Spark sparks[NUM_SPARKS];


/** Agent Clock Variables **/
uint8_t clock_agents_here[NUM_LEDS];
// start with moody arlene + deki clock
AgentClock aclock(darkpurples, blues, greens, NUM_LEDS, clock_agents_here, BLENDING, BRIGHTNESS);
CRGBPalette16 bgpalette = adbasic;
uint8_t clocktype = 0; //goth clock = 0; arlene clock = 1

/**Binary Clock Variables **/
BinaryClock bclock(CRGB::White, CRGB::Black, BLENDING,BRIGHTNESS);


void setup() {

  //Set up the LEDs
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );
  for (int i = 0; i < NUM_LEDS; i++) //make them all red so we can tell it rebooted
    leds[i] = CRGB::Red;
  FastLED.show();
  delay(2000);//show red for 2 secs so i know it rebooted.
  for (int i = 0; i < NUM_LEDS; i++) //make them all black
    leds[i] = CRGB::Black;

    
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
    agents[i].initagent(ColorFromPalette(adbasic, random(1,255)), pos, random(1,10));
    agents_here[pos] ++;
  }
  
  for (int i = 0; i < NUM_SPARKS; i++)
  {
    uint8_t pos = random(1, NUM_LEDS);
    sparks[i].initspark(ColorFromPalette(whites,random(1,255)), pos,  random(1,10), random(20,50));
    
  }

}

void draw_sparks(Spark p_sparks[])
{
  for(int i = 0; i < NUM_SPARKS; i++)
  {
    leds[p_sparks[i].pos] = p_sparks[i].color;
    p_sparks[i].fade();
  }
  
}


 uint8_t r ;
static CRGB color = ColorFromPalette(adbasic, random(1,255));
void loop()
{

  
  FastLED.show(); 
  FastLED.delay(1000 / UPDATES_PER_SECOND);

ArleneDekiBasicMode();



}




//run through different things for 10 seconds each.
void ArleneDekiBasicMode()
{
  /** PG: I can't decide how I want to switch between the different agent clocks at this point. 
   *  hacked together a thingie to make it switch between different effects. 
   */
   static uint8_t start_index = 0; 

  r = random(1,30);

  start_index++;
        
  FastLED.show(); 
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  
//Agent Clock
     for(int i = 0 ; i < UPDATES_PER_SECOND * 10; i++)
     {
      start_index++;
      aclock.drawagentclock(clock_agents_here, leds, 1, bgpalette, start_index);
       FastLED.show(); 
       FastLED.delay(1000 / UPDATES_PER_SECOND);
     } 
     
    switchclock(); //switch palette so next time clock colors are diff.

//Binary Clock
        for(int i = 0 ; i < UPDATES_PER_SECOND * 10; i++)
        {
          start_index++;
          bclock.drawbinaryclock(leds, adbasic, start_index, NUM_LEDS);
          FastLED.show(); 
          FastLED.delay(1000 / UPDATES_PER_SECOND);
        }

//Random Agents
        for(int i = 0 ; i < UPDATES_PER_SECOND * 10; i++)
        {
          move_agents(agents, agents_here);
          draw_agents(agents, agents_here);
          FastLED.show(); 
          FastLED.delay(1000 / UPDATES_PER_SECOND);
        }

   for(int i = 0 ; i < UPDATES_PER_SECOND * 10; i++)
        {
         
draw_sparks(sparks);
 FastLED.show(); 
  FastLED.delay(1000 / UPDATES_PER_SECOND);

        }
}

void switchclock()
{
  if(clocktype==0) //go from gothy to Arlene clock
  {
    aclock.sec_palette = pinks;
    aclock.min_palette = yellows;
    aclock.hour_palette = whites;
    bgpalette = abasic;
    clocktype=1;
  }
  else //arlene to gothy clock
    {
    aclock.sec_palette = darkpurples;
    aclock.min_palette = blues;
    aclock.hour_palette = greens;
    bgpalette = adbasic;
    clocktype = 0;
  }

  aclock.recoloragents();
  
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


/*** BEGINNINGS OF FRACTIONAL CLOCK START HERE ...
 *  ... it ended up being pretty slow/less interesting to look at. 
 *  maybe can be salvaged 
 */

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

/***** OLD CODE *****/
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












