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
#include "agents.h"
#include "ad_palettes.h"
#define DATA_PIN     17
#define NUM_LEDS    114
#define BRIGHTNESS  32 //took brightness down by 32 PG


#define NUM_AGENTS 50
#define UPDATES_PER_SECOND 100

CRGB leds[NUM_LEDS];
uint8_t agents_here[NUM_LEDS];

CRGBPalette16 currentPalette; //assign before palette color grabbing
TBlendType    currentBlending;

struct agent_s agents[NUM_AGENTS];

void setup() {

  //Set up the LEDs
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );
    
  //set up random numbers
  randomSeed(analogRead(0));
  randomSeed(analogRead(random(0, 7)));

  
  currentPalette = adbasic; //basic ad colors
  currentBlending = LINEARBLEND;
    
  //Initialize some agents
  for (int i = 0; i < NUM_AGENTS; i++)
  {
    int8_t rdir = random(1, 6); //randomize directions
    if (rdir > 3)
      rdir =  - 1;
    else
      rdir = 1;
      init_agent(&agents[i], currentPalette, random(1,256), random(0, NUM_LEDS), rdir, 4, NULL);
  }
}


void loop()
{
  ChangePalettePeriodically();
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; //motion speed 
  FillLEDsFromPaletteColors( startIndex); 
  
  
  
  //30 agents in palette of blue/green/purple move randomly
 /* draw_agents();
  FastLED.show();
  move_agents();*/
  
  
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  }

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
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
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
       // if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
      //  if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = darkpurples;   currentBlending = LINEARBLEND; }
  if( secondHand == 30)  { currentPalette = adbasic;       currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { currentPalette = greens;           currentBlending = LINEARBLEND; }
     //   if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
      //  if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}



/****
 * Basic set of purple/blue/green/black/white
 * reasonably bright
 **/
CRGBPalette16 SetupADPalette1()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    CRGB blue = CHSV( HUE_BLUE, 255,255);
    CRGB white = CRGB::White;
    
    return CRGBPalette16(
                                   purple,  purple,  purple ,  purple,
                                   blue, blue, blue,  green,
                                   green,  purple,  purple,  purple,
                                   blue, blue, green,  green );
                                  
}
                                  
                                   



/****
 * Basic, initialize an agent that just has a color 
 */
void init_agent(struct agent_s* toinit, CRGB color1, int16_t posi, uint8_t dir, uint8_t maxw, void *fptr)
{
  toinit->color1 = color1;
  toinit->pos = posi;
  agents_here[posi]++;
  toinit->dir = dir;
  toinit->waited = 0;
  toinit->max_wait = maxw;
  toinit->span = 1;
  toinit->agent_type = AGENT_CLASSIC;

  //not using palette make it null
  toinit->palette = NULL;
  toinit->col_index=0;
}

/****
 * Initialize an agent that uses a palette
 */
void init_agent(struct agent_s* toinit, CRGBPalette16 palette, uint8_t col_index, int16_t posi, uint8_t dir, uint8_t maxw, void *fptr)
{
  toinit->palette = palette;
  toinit->col_index = col_index;
  toinit->color1 = ColorFromPalette( palette, col_index, BRIGHTNESS, currentBlending);
  toinit->pos = posi;
  agents_here[posi]++;
  toinit->dir = dir;
  toinit->waited = 0;
  toinit->max_wait = maxw;
  toinit->span = 1;
  toinit->agent_type = AGENT_CLASSIC;

  //not using palette make it null
  toinit->palette = NULL;
  toinit->col_index=0;
}

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

void update_agents_colors()
{
  for(int i = 0; i < NUM_AGENTS; i++)
    update_agent_color(&agents[i]);
}

/***** This doesn't work 

For some reason ColorFromPalette bars when I try to update. 
If I give it a palette directly its ok modulo the below issue. (e.g., RainbowColors_p)

For some reason all the agents end up in lock step for color
even if they start off with different colors. 

If I try to set palette in the agent to be RainbowColors and pass the palette
in to the ColorFromPalette function they end up all in lock step +
iterating through blue/purple/white (at least for RainbowColors palette).

******/

void update_agent_color(struct agent_s* toupdate)
{
   if(toupdate->palette != NULL)
    {
      CRGBPalette16 palette = toupdate->palette;
      toupdate->col_index = toupdate->col_index + 1;
      uint8_t tmp = toupdate->col_index;
     // currentPalette = toupdate->palette;
      toupdate->color1 = ColorFromPalette(palette, tmp, BRIGHTNESS, currentBlending);
    
    }
}

void move_agents()
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
    toupdate->waited = 0;
  }
  else
    toupdate->waited++;
}

boolean sameDirection(struct agent_s* a, struct agent_s* b)
{
  return ~(0x80 & (a->dir ^ b->dir));
}

void draw_agents()
{
  for (int i = 0; i < NUM_AGENTS; i++)
    leds[agents[i].pos]=agents[i].color1;
  //  strip.setPixelColor(agents[i].pos, agents[i].rgb[0], agents[i].rgb[1], agents[i].rgb[2]);
  for(int i = 0; i < NUM_LEDS; i++)
  {
     if(agents_here[i] == 0)
      leds[i]=CRGB::Black;
  }
  
 
}



// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};



// Additionl notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes.
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact 
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved 
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from green-to-blue, and then Blue.
