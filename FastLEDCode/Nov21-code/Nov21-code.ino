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
#define DATA_PIN     17
#define NUM_LEDS    114
#define BRIGHTNESS  64

/* Different types of agent */
#define AGENT_DISABLED 0
#define AGENT_CLASSIC 1
#define AGENT_COMET 2

#define NUM_AGENTS 30
#define UPDATES_PER_SECOND 100

CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

/*wandering agents*/
struct agent_s {
  uint8_t agent_type;
  CRGB color1; // Primary RGB color
  CRGB color2; // Secondary RGB color
  int16_t pos; // Position (literal or seed)
  uint8_t span; // Width or pixel count
  int8_t dir; // Direction (signed)
  uint16_t waited; // State counter for wait
  uint16_t max_wait;  // Max wait
};

struct agent_s agents[NUM_AGENTS];

void setup() {
    delay( 3000 ); // power-up safety delay
   FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(  BRIGHTNESS );
    
    currentPalette = CloudColors_p;  
    currentBlending = LINEARBLEND;


     randomSeed(analogRead(0));
  randomSeed(analogRead(random(0, 7)));

  for (int i = 0; i < NUM_AGENTS; i++)
  {
    int8_t junk = random(1, 6);
    if (junk > 3)
      junk =  - 1;
    else
      junk = 1;
    int otherjunk = random(1, 10);
    if (otherjunk < 3)
      init_agent(&agents[i], CRGB::Red, random(0, NUM_LEDS), junk, random(6, 20), NULL);
    else if (otherjunk < 6)
      init_agent(&agents[i], CRGB::HotPink, random(0, NUM_LEDS), junk, random(14, 20), NULL);
    else
      init_agent(&agents[i], CRGB::Violet, random(0, NUM_LEDS), junk, random(20, 27), NULL);
  }
}


void loop()
{
    /*ChangePalettePeriodically();
    
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    
  //  FillLEDsFromPaletteColors( startIndex);
      draw_agents();
      clear_agents();
      move_agents();
      
   // FastLED.show();
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
       // if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
     if( secondHand == 25)  { SetupPurpleAndGreenPalette();             currentBlending = NOBLEND; }
       // if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupPBGPalette();       currentBlending = NOBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
  if( secondHand == 45)  { SetupPBGPalette();       currentBlending = LINEARBLEND; }
      //  if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
     //   if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
      //  if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    CRGB blue = CHSV( HUE_BLUE, 255,255);
    CRGB white = CRGB::White;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  blue ,  blue,
                                   purple, purple, black,  black,
                                   green,  green,  white,  white,
                                   purple, purple, black,  black );
}

// This function sets up a palette of purple blue and green.
void SetupPBGPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    CRGB blue = CHSV( HUE_BLUE, 255,255);
    CRGB white = CRGB::White;
    
    currentPalette = CRGBPalette16(
                                   purple,  purple,  purple ,  purple,
                                   blue, blue, blue,  green,
                                   green,  purple,  purple,  purple,
                                   blue, blue, green,  green );
                                   
}


void init_agent(struct agent_s* toinit, CRGB color1, int16_t posi, uint8_t dir, uint8_t maxw, void *fptr)
{
  toinit->color1 = color1;
  toinit->pos = posi;
  toinit->dir = dir;
  toinit->waited = 0;
  toinit->max_wait = maxw;
  toinit->span = 1;
  toinit->agent_type = AGENT_CLASSIC;
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

/*void set_agent_rgb(struct agent_s* toset, uint8_t red, uint8_t blue, uint8_t green)
{
  toset->rgb[0] = red;
  toset->rgb[1] = blue;
  toset->rgb[2] = green;
} 

void set_agent_rgb2(struct agent_s* toset, uint8_t red, uint8_t blue, uint8_t green)
{
  toinit->rgb2[0] = red;
  toinit->rgb2[1] = blue;
  toinit->rgb2[2] = green;
} 
*/

void clear_agents()
{
  for (int i = 0; i < NUM_AGENTS; i++)
  {
    if(agents[i].agent_type == AGENT_CLASSIC) leds[agents[i].pos]=CRGB::Black;//, 0, 0, 0);
    if(agents[i].agent_type == AGENT_COMET) 
    {
      leds[agents[i].pos]=CRGB::Black;
      //strip.setPixelColor(agents[i].pos, 0, 0, 0);
      if(agents[i].dir > 0)
      {
         for(int j = 1; j < agents[i].span; j++)
         {
           leds[agents[i].pos-j]=CRGB::Black;//, 0, 0, 0);
         }
      }
      else
      {
         for(int j = 1; j < agents[i].span; j++)
         {
           leds[agents[i].pos+j]=CRGB::Black;//, 0, 0, 0);
         }
      }
    }
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
    
    toupdate->pos += toupdate->dir;
    while (toupdate->pos < 0)
      toupdate->pos += NUM_LEDS;
    toupdate->pos %= NUM_LEDS;

    /* collision detection. may or may not work */
    for (int i = 0; i < NUM_AGENTS; i++)
    {

      if (agents[i].pos == toupdate->pos && (sameDirection(&agents[i], toupdate) ))
      {
        toupdate->dir *= -1;
        toupdate->pos = (toupdate->pos + 1) % NUM_LEDS;
      }
    }

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

  FastLED.show();
 // strip.show();
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
