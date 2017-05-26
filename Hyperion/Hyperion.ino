/*  OctoWS2811 BasicTest.ino - Basic RGB LED Test
    http://www.pjrc.com/teensy/td_libs_OctoWS2811.html
    Copyright (c) 2013 Paul Stoffregen, PJRC.COM, LLC

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including        The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.

  Required Connections
  --------------------
    pin 2:  LED Strip #1    OctoWS2811 drives 8 LED Strips.
    pin 14: LED strip #2    All 8 are the same length.
    pin 7:  LED strip #3
    pin 8:  LED strip #4    A 100 ohm resistor should used
    pin 6:  LED strip #5    between each Teensy pin and the
    pin 20: LED strip #6    wire to the LED strip, to minimize
    pin 21: LED strip #7    high frequency ringining & noise.
    pin 5:  LED strip #8
    pin 15 & 16 - Connect together, but do not use
    pin 4 - Do not use
    pin 3 - Do not use as PWM.  Normal use is ok.

  This test is useful for checking if your LED strips work, and which
  color config (WS2811_RGB, WS2811_GRB, etc) they require.
*/

#define USE_OCTOWS2811
#include<OctoWS2811.h>
#include<FastLED.h>
#include "Palettes.h"
#include "Spark.h"
#include "FatAgent.h"

#define UPDATES_PER_SECOND 16//30 //60
#define BRIGHTNESS  16 //took brightness down by 32 PG
#define BLENDING LINEARBLEND
#define LEDS_PER_STRIP 600 //480
#define NUM_STRIPS  4 //8
#define ROW_LENGTH  52
#define NUM_ROWS  47

#define SPARK 0
#define UNDULATE 1
#define DOWNAGENT 2
#define DIAGONAL 3
#define FIZZLE 4

#define BUTTON1 12
#define BUTTON2 11
#define BUTTON3 10
#define BUTTON4 9
#define BUTTON5 4

#define KNOB4 12
#define KNOB3 15


CRGB leds[NUM_STRIPS * LEDS_PER_STRIP];


// 6 rows per strip for debugging
const int NUM_LEDS = LEDS_PER_STRIP * NUM_STRIPS;

CRGBPalette16 currentPalette; //assign before palette color grabbing
TBlendType    currentBlending;



CRGBPalette16 getRandomPalette(byte r, byte g, byte b);

/** generic spark population variables **/
#define NUM_SPARKS 600 //define sparks on a per line basis
Spark sparks[NUM_SPARKS];
uint8_t sparkcol_idx = 1;
CRGBPalette16 sparkpalette = HeatColors_p;

/** FatAgents **/
#define NUM_FAT_AGENTS 75
FatAgent fatagents[NUM_FAT_AGENTS];
FatAgent test;

//CRGBPalette16 gothy_palettes[] = {adbasic, darkpurples, bluespurples, blues};
//CRGBPalette16 random_reds[] = {getRandomPalette(0xFF, 0, 0), getRandomPalette(0xFF, 0, 0), getRandomPalette(0xFF, 0, 0), getRandomPalette(0xFF, 0, 0)};
//CRGBPalette16 random_purples[] = {getRandomPalette(0xFF, 0, 0xFF), getRandomPalette(0xFF, 0, 0xFF), getRandomPalette(0xFF, 0, 0xFF), getRandomPalette(0xFF, 0, 0xFF)};
//CRGBPalette16 random_bluegreens[] = {getRandomPalette(0x00, 0xFF, 0xFF), getRandomPalette(0x00, 0xFF, 0xFF), getRandomPalette(0x00, 0xFF, 0xFF), getRandomPalette(0x00, 0xFF, 0xFF)};


CRGBPalette16 colors[] = {HeatColors_p, RainbowColors_p, ForestColors_p,whites,getRandomPalette(0xFF, 0, 0),
                          CloudColors_p, abasic, adbasic, purplegreen, darkpurples,whites,
                          bluespurples, blues, greens, yellows, pinks, randomRainbow, randomRed,getRandomPalette(0x00, 0xFF, 0xFF)
                         };
int numPalettes = 19;

void setup() {

  //Initialize controller box
  pinMode( BUTTON1, INPUT);
  pinMode( BUTTON2, INPUT);
  pinMode( BUTTON3, INPUT);
  pinMode( BUTTON4, INPUT);
  pinMode( BUTTON5, INPUT);

  Serial.begin(9600);

  randomSeed(analogRead(0));
  randomSeed(analogRead(random(0, 7)));

  LEDS.addLeds<OCTOWS2811>(leds, LEDS_PER_STRIP);
  FastLED.setBrightness(  BRIGHTNESS );

  //init sparks
  for (int i = 0; i < NUM_SPARKS; i++)
    sparks[i].initspark(ColorFromPalette(sparkpalette, random(1, 255)), random(1, LEDS_PER_STRIP - 1),  random(1, 20), random(5, 15));

  //init fat agents
  for (int i = 0; i < NUM_FAT_AGENTS; i++)
  {
    int x = random(0, ROW_LENGTH);
    int y = random(0, NUM_ROWS);
    fatagents[i].initagent(ColorFromPalette(sparkpalette, random(1, 255)), x, y, random(1, 5));
    fatagents[i].setspan(random(2, 16));
    fatagents[i].dir = random(0, 1); //make them go the diff way.
  }


  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = 0xaaaaaa;
  LEDS.show();
  LEDS.delay(1000);

  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = 0x000000;

  LEDS.show();
  LEDS.delay(1000);

}


int curr_buttons[5] = {0, 0, 0, 0, 0};
int palette_idx = 0;
int mode = 0;
//per mode parameters
int diagonal_gap = 3;
int palette_stretch = 5;
int palette_delay = 100;
int perc_sparks = 100;
void loop() {

  int last_buttons[5] = {0, 0, 0, 0, 0};
  for (int i = 0; i < 5; i++)
    last_buttons[i] = curr_buttons[i];

  getButtons();


  boolean reinit = false;
  if (last_buttons[0] != curr_buttons[0])
  {
    palette_idx++;
    palette_idx %= 15;
    reinit = true;
  }
  if (last_buttons[1] != curr_buttons[1])
  {
    mode++;
    mode %= 4;
    reinit = true;
  }
  if (last_buttons[2] != curr_buttons[2])
  {
    palette_delay += 10;
    palette_delay %= 1000;
  }
  if (last_buttons[3] != curr_buttons[3])
  {
    palette_delay -= 10;
    if (palette_delay < 0)
      palette_delay = 0;
  }
  if (last_buttons[4] != curr_buttons[4])
  {
    if (mode == SPARK)
    {
      perc_sparks -= 10;
      if (perc_sparks == 0)
        perc_sparks = 100;
    }
    if (mode == DIAGONAL)
    {
      diagonal_gap++;
      diagonal_gap %= 50;
    }
  }


  if (mode == SPARK)
    sparkmode(colors[palette_idx], palette_delay, perc_sparks, 10, 15, reinit);
  if (mode == UNDULATE)
    undulate(colors[palette_idx], palette_stretch, palette_delay);
  if (mode == DOWNAGENT)
    downfatagentmode(colors[palette_idx], palette_delay,  reinit);
  if (mode == DIAGONAL)
    diagonals(colors[palette_idx], 5, palette_delay, diagonal_gap);

  //   diagonals(colors[palette_idx], 5, 100, diagonal_gap);
  // undulate(colors[palette_idx], 5, 100);
  // sparkmode(colors[palette_idx], 100, 100, 10, 15, reinit);


}

void getButtons()
{
  curr_buttons[0] = digitalRead(BUTTON1);
  curr_buttons[1] = digitalRead(BUTTON2);
  curr_buttons[2] = digitalRead(BUTTON3);
  curr_buttons[3] = digitalRead(BUTTON4);
  curr_buttons[4] = digitalRead(BUTTON5);
}
void randomCool1()
{
  diagonals(HeatColors_p, random(1, 10), 100, random(2, 50));

}
void wipeColor(int color)
{
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = color;
  LEDS.show();
  LEDS.delay(1000);
}
void runCycle(int mode, CRGBPalette16 palette, boolean reinit)
{


  int palette_delay = 100;
  if (mode == SPARK)
    sparkmode(palette, palette_delay, 100, 10, 15, reinit);
  else if (mode == UNDULATE)
    undulate(palette, palette_stretch, palette_delay);
  else if (mode == DOWNAGENT)
    downfatagentmode(palette, palette_delay,  reinit);
  else if (mode == DIAGONAL)
    diagonals(palette, 5, palette_delay, diagonal_gap);



}

void fizzle(CRGBPalette16 palette, int palette_delay, int duration)
{
  int fizval = 100;
  while (fizval > 75)  {
    sparkmode(palette, palette_delay, fizval, 10, 15, false);
    fizval -= 5;
  }
  while (fizval > 25)  {
    sparkmode(palette, palette_delay, fizval, 10, 15, false);
    fizval -= 2;
  }
  while (fizval >= 0)
  {
    sparkmode(palette, palette_delay, fizval, 10, 15, false);
    fizval--;
  }


}



CRGBPalette16 getRandomPalette(byte r, byte g, byte b)
{
  CRGBPalette16 toreturn;
  for (int i = 0; i < 16; i++)
  {
    toreturn.entries[i].r = 0;
    toreturn.entries[i].g = 0;
    toreturn.entries[i].b = 0;
    if (r > 0)
      toreturn.entries[i].r = random(10, r);
    if (g > 0)
      toreturn.entries[i].g = random(10, g);
    if (b > 0)
      toreturn.entries[i].b = random(10, b);
  }


  return toreturn;
}

int diag_iterations = 0;
int undulate_length = 0;
int curr_pix = 0;

void diagonals(CRGBPalette16 palette, int palette_stretch, int palette_delay, int gap)
{

  gap %= 50; //don't do nonsense gap values

  int curr_offset = diag_iterations % ROW_LENGTH;

  for (int i = 0; i < NUM_LEDS; i++)
  {
    if (i < (NUM_LEDS - curr_offset))
    {
      if (i % gap == 0)
      {
        leds[i + curr_offset] = ColorFromPalette(palette, undulate_length + (i / palette_stretch)); ///palette_stretch);
      }
      else
        leds[i + curr_offset] = CRGB::Black;
    }

  }

  undulate_length++;
  undulate_length %= 255;
  diag_iterations++;

  if (diag_iterations > ROW_LENGTH)
    diag_iterations = 0;
  if (diag_iterations < 0)
    diag_iterations = 0;

  FastLED.delay(palette_delay);
  FastLED.show();


}

//Potential parameters:
// length of palette (ie., what we divide i by)
// palette ~8 palettes to choose from
// speed how long we delay by between iterations
// reasonable values: 10,10,500
//global var
void undulate(CRGBPalette16 palette, int palette_stretch, int palette_delay)
{
  //clear strip
  // for(int i = 0; i< NUM_LEDS;i++)
  //  leds[i]=0x000000;



  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(palette, undulate_length + (i / palette_stretch));
  }

  undulate_length++;
  undulate_length %= 255;

  FastLED.delay(palette_delay);
  FastLED.show();

}

//parameters, palette, speed, would need to change th code, but number of agents maybe , also their directions
void downfatagentmode(CRGBPalette16 palette, int palette_delay, boolean reinit)
{
  if (reinit)
  {
    for (int i = 0; i < NUM_LEDS; i++)
      leds[i] = 0x000000;


    for (int i = 0; i < NUM_FAT_AGENTS; i++)
    {
      int x = random(0, ROW_LENGTH);
      int y = random(0, NUM_ROWS);
      fatagents[i].initagent(ColorFromPalette(palette, random(1, 255)), x, y, random(1, 5));
      // fatagents[i].setspan(random(2, 16));
      fatagents[i].setcolor(ColorFromPalette(palette, random(1, 255)));
      fatagents[i].dir = random(0, 1); //make them go down.
    }
  }

  move_fatagents(fatagents);
  clear_fatagents();

  draw_fatagents(fatagents);

  FastLED.show();
  FastLED.delay(palette_delay);


}





void sparkmode(CRGBPalette16 palette, int palette_delay, int perc_sparks, int min_fade_val, int max_fade_val, boolean reinit)
{

  if (reinit)
  {
    for (int i = 0; i < NUM_LEDS; i++)
      leds[i] = 0x000000;
  }
  //init the sparks
  for (int i = 0; i < NUM_SPARKS; i++)
  {
    if (reinit)
      sparks[i].reinit(ColorFromPalette(palette, random(1, 255)), random(1, LEDS_PER_STRIP - 1),  random(1, 20), random(min_fade_val, max_fade_val));

    if (random(1, 100) < perc_sparks )
    {

      sparks[i].alive = true;

    }
    else
    {
      sparks[i].color = black;
      sparks[i].alive = false;
      drawSpark(sparks[i]);
    }
  }


  draw_sparks(sparks);
  FastLED.delay(palette_delay);
  FastLED.show();

  for (int i = 0; i < NUM_SPARKS; i++)
  {
    if (sparks[i].alive)
    {
      if (sparks[i].color.r == 0 && sparks[i].color.g == 0 && sparks[i].color.b == 0)
      {

        sparks[i].color = ColorFromPalette(palette, random(1, 255));
        int numFades = random(0, 5);
        for (int f = 0; f < numFades; f++)
          sparks[i].fade();

      }
    }




  }
}


/*********** Here be utility functions **********/
void draw_sparks(Spark p_sparks[])
{

  for (int i = 0; i < NUM_SPARKS; i++)
  {
    if (p_sparks[i].alive || (sparks[i].color.r > 0 || sparks[i].color.g > 0 || sparks[i].color.b > 0))
    {
      drawSpark(p_sparks[i]);

      p_sparks[i].fade();
    }


  }


}

//draw it on all  strips
void drawSpark(Spark p_spark)
{
  for (int j = 0; j < NUM_STRIPS; j++)
  {
    if (j * LEDS_PER_STRIP + p_spark.pos < 0)
      colorPixel(0, p_spark.color);
    else if (j * LEDS_PER_STRIP + p_spark.pos > NUM_LEDS)
      colorPixel(NUM_LEDS - 1, p_spark.color);
    else
      colorPixel(p_spark.pos + j * LEDS_PER_STRIP, p_spark.color);
  }
}

void colorPixel(int x, CRGB color)
{
  leds[x] = color;
}


void colorPixel(int x, int y, CRGB color)
{
  leds[y * ROW_LENGTH + x] = color;
}

void move_fatagents(FatAgent p_fatagents[])
{
  for (int i = 0; i < NUM_FAT_AGENTS; i++)
  {
    p_fatagents[i].move(NULL, ROW_LENGTH, NUM_ROWS, NUM_LEDS);
  }

}

void draw_fatagents(FatAgent p_fatagents[])
{
  for (int i = 0; i < NUM_FAT_AGENTS; i++)
  {
    p_fatagents[i].draw(leds, ROW_LENGTH, NUM_ROWS, NUM_LEDS);
  }
}

void clear_fatagents()
{
  memset(leds, 0, 3 * NUM_LEDS);
}


void gameOfLife(CRGB w, CRGB b)
{
  //CRGB next[NUM_LEDS];

  for (int i = 0; i < ROW_LENGTH; i++)
  {
    for (int j = 0; j < NUM_ROWS; j++)
    {

      // if((j*ROW_LENGTH + i) < NUM_LEDS)
      // {
      if (countLiveNeighbors(i, j, w) > 0)
      {
        colorPixel(i, j, chartreuse);

      }
      // }
    }
  }

  FastLED.show();
  FastLED.delay(2000);

}

int countLiveNeighbors(int x, int y, CRGB w)
{
  CRGB p;
  int liveneighbors = 0;
  for (int i = x - 1; i <= x + 1; i++)
  {
    if (i < 0)
      i = 0;
    if (i >= ROW_LENGTH)
      i = ROW_LENGTH - 1;
    if (y > 0) // grab the row below
    {
      p = getPixel(i, y - 1);
      if (p.r == w.r && p.g == w.g && p.b == w.b)
        liveneighbors++;
    }
    if (y < NUM_ROWS - 1) //grab the row above
    {
      p = getPixel(i, y + 1);
      if (p.r == w.r && p.g == w.g && p.b == w.b)
        liveneighbors++;
    }
  }

  if (x > 0)
  {
    p = getPixel(x - 1, y);
    if (p.r == w.r && p.g == w.g && p.b == w.b)
      liveneighbors++;
  }
  if (x < ROW_LENGTH - 1)
  {
    p = getPixel(x + 1, y);
    if (p.r == w.r && p.g == w.g && p.b == w.b)
      liveneighbors++;
  }

  return liveneighbors;
}

CRGB getPixel(int x, int y)
{
  //bounds checks
  if ((y * ROW_LENGTH + x) < NUM_LEDS)
  {
    if (y >= 0 && y < ROW_LENGTH && x >= 0 && x < NUM_ROWS )
    {
      return leds[y * ROW_LENGTH + x];

    }
  }
  return black;
}




