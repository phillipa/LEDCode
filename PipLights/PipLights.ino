#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <stdint.h>

#define PIN 17

#define MAX(x,y) ((x)>(y))?x:y
#define ADD_CEIL8(x,inc) (255-x)>(inc)?(x+inc):(255)
#define ADD_CEIL(x,inc,maxnum) (maxnum-x)>(inc)?(x+inc):maxnum
#define SUB_FLOOR8(x,dec) (x)<(dec)?(0):(x-dec)

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
#define NUM_LEDS 114
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
#define NUM_AGENTS 30
#define NUM_FLASHES 6
#define DEFAULT_BASEVAL 0
#define DEFAULT_MAXVAL 128
/*wandering agents*/
struct agent_s {
  uint8_t agent_type;
  uint8_t rgb[3]; // Primary RGB color
  uint8_t rgb2[3]; // Secondary RGB color
  int16_t pos; // Position (literal or seed)
  uint8_t span; // Width or pixel count
  int8_t dir; // Direction (signed)
  uint16_t waited; // State counter for wait
  uint16_t max_wait;  // Max wait
};

#define AGENT_DISABLED 0
#define AGENT_CLASSIC 1
#define AGENT_COMET 2

void init_agent(struct agent_s* toinit, uint8_t red, uint8_t blue, uint8_t green, int16_t posi, uint8_t dir, uint8_t maxw, void *fptr)
{
  toinit->rgb[0] = red;
  toinit->rgb[1] = blue;
  toinit->rgb[2] = green;
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

void set_agent_rgb(struct agent_s* toset, uint8_t red, uint8_t blue, uint8_t green)
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

/* pip flash */
struct flash_s {
  boolean on; //is the flash on right now or not
  boolean fade; //fade out vs. timed exit
  uint16_t max_wait; //how long to stay if not fade
  uint16_t waited;
  uint8_t seed; //random seed to generate positions
  uint8_t rgb[3];
  uint8_t orig_rgb[3];
  uint8_t num_pixels;
};

struct rgb_s
{
  uint8_t rgb[3];
} ;

struct rgb_s my_rgb; //for testing.
struct agent_s agents[NUM_AGENTS];
struct flash_s flashes[NUM_FLASHES];

void setup() {
  Serial.begin(9600);
  Serial.println("BOOT");
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  // Some example procedures showing how to display to the pixels:

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
      init_agent(&agents[i], 255, 0, 0, random(0, NUM_LEDS), junk, random(6, 20), NULL);
    else if (otherjunk < 6)
      init_agent(&agents[i], 0, 255, 0, random(0, NUM_LEDS), junk, random(14, 20), NULL);
    else
      init_agent(&agents[i], 0, 0, 255, random(0, NUM_LEDS), junk, random(20, 27), NULL);
  }

  //  for(int i = 0; i<NUM_FLASHES; i++)
  //{
  // int8_t junk = random(0,11);
  // if(junk<5)
  //   init_flash(&flashes[2],  true, true,  200, random(150,255), 0, 0, 16);
  //  else
  //  init_flash(&flashes[1],  false, true,  200, 0, random(200,255), 0, 16);
  init_flash(&flashes[0],  true, true,  random(150, 300), 255, 0, 0, 16);
  init_flash(&flashes[1],  true, false,  random(150, 300), 255, 255, 0, 16);
  init_flash(&flashes[2],  true, true,  random(150, 300), 0, 255, 0, 16);
  init_flash(&flashes[3],  true, true,  random(150, 300), 0, 255, 255, 16);
  init_flash(&flashes[4],  true, false,  random(150, 300), 0, 0, 255, 16);
  init_flash(&flashes[5],  true, true,  random(150, 300), 255, 0, 255, 16);
}

int next;
void loop() {
  rainbowsAndAgents();
}

/** the random rainbows, agents and dance break program**/
void rainbowsAndAgents()
{
  int action = random(1, 101);

  if (action  < 5)
    wipeRainbow(3);
  else if (action < 10)
  {
    for (int i = 0; i < 500; i++)
      danceBreak();
  }
  else if (action < 45)
  {
    for (int i = 0; i < 15000; i++)
    {
      draw_agents();
      clear_agents();
      move_agents();
    }
  }

  else
  {
   // for (int i = 0; i < 20; i++)
    //{
      uint32_t count = 0;
      uint32_t next = 0;
      while (count < 250){
          next = makeRainbow(random(0, 255), random(51, 255), 52, next); //random(5, 52), 0);
    count++;  
    }
  //  }
  }
  delay(3);

}

void danceBreak()
{
  uint8_t r = random(50, 150);
  uint8_t g = random(50, 150);
  uint8_t b = random(50, 150);

  for (int i = 0; i < NUM_LEDS; i++)
  {
    if ((i % random(1, 6)) == 0)
    {

      strip.setPixelColor(i, r, g, b);
    }
    else
      strip.setPixelColor(i, 0, 0, 0);

  }

  strip.show();
}
void clear_agents()
{
  for (int i = 0; i < NUM_AGENTS; i++)
  {
    if(agents[i].agent_type == AGENT_CLASSIC) strip.setPixelColor(agents[i].pos, 0, 0, 0);
    if(agents[i].agent_type == AGENT_COMET) 
    {
      strip.setPixelColor(agents[i].pos, 0, 0, 0);
      if(agents[i].dir > 0)
      {
         for(int j = 1; j < agents[i].span; j++)
         {
           strip.setPixelColor(agents[i].pos-j, 0, 0, 0);
         }
      }
      else
      {
         for(int j = 1; j < agents[i].span; j++)
         {
           strip.setPixelColor(agents[i].pos+j, 0, 0, 0);
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

void draw_agents()
{
  for (int i = 0; i < NUM_AGENTS; i++)
    strip.setPixelColor(agents[i].pos, agents[i].rgb[0], agents[i].rgb[1], agents[i].rgb[2]);

  strip.show();
}

void init_flash(struct flash_s* toinit, boolean on, boolean fade, uint16_t max_wait, uint8_t red, uint8_t green, uint8_t blue, uint8_t num_pixels)
{
  toinit->on = on;
  toinit->fade = fade;
  toinit->max_wait = max_wait;
  toinit->waited = 0;
  toinit->seed = random(0, 255);
  toinit->rgb[0] = red;
  toinit->rgb[1] = green;
  toinit->rgb[2] = blue;
  toinit->orig_rgb[0] = red;
  toinit->orig_rgb[1] = green;
  toinit->orig_rgb[2] = blue;
  toinit->num_pixels = num_pixels;
}

void update_flash(struct flash_s* me)
{
  if (me->fade == true && me->on == true)
  {

    uint16_t fade_interval = me->max_wait >> 3;
    if (me->waited % fade_interval == 0)
    {
      me->rgb[0] >>= 3;
      me->rgb[1] >>= 3;
      me->rgb[2] >>= 3;
    }
    me->waited++;

    if ((me->waited >= me->max_wait) || (me->rgb[0] <= 0 && me->rgb[1] <= 0 && me->rgb[2] <= 0))
    {
      me->on = false; //we are off now
      me->rgb[0] = me->orig_rgb[0];
      me->rgb[1] = me->orig_rgb[1];
      me->rgb[2] = me->orig_rgb[2];
      // me->seed = random(0,11);
      me->waited = 0;

    }

  }
  else if (me->on && !me->fade)
  {
    me->waited++;
    if (me->waited == me->max_wait) //on long enough turn off.
    {
      me->waited = 0;
      me->on = false;
    }
  }
  else if (me->on == false) //we are off, see if we've waited long enough to come back on
  {
    me->waited++;
    if (me->waited >= me->max_wait)
    {
      me->waited = 0;
      me->on = true;  //waited long enough turn back on
    }
  }
}

void draw_flash(struct flash_s* me)
{

  randomSeed(me->seed);
  for (int i = 0 ; i < me->num_pixels ; i++)
  {
    uint8_t p = random(0, NUM_LEDS + 1);
    if (me->on) //only draw me if i am on.
      strip.setPixelColor(p, me->rgb[0], me->rgb[1], me->rgb[2]);
    else
      strip.setPixelColor(p, 0, 0, 0);


  }
  //reset good randomness
  randomSeed(analogRead(0));
  randomSeed(analogRead(random(0, 7)));

}

void move_agent(struct agent_s* toupdate)
{
  if (toupdate->waited == toupdate->max_wait)
  {
    if (toupdate->my_function)
    {

    }
    else nextRainbow((struct rgb_s *)toupdate->rgb, 0, 255, 51);  //hardcode rainbow setting for now.
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


uint32_t colToInt(uint8_t r, uint8_t g, uint8_t b)
{
  return (r << 16 & g << 8 & b);
}

void incRainbow(struct agent_s *me, uint8_t baseval, uint8_t maxval, uint8_t inc)
{
  nextRainbow((struct rgb_s *)me, baseval, maxval, inc);
}

void nextRainbow(struct rgb_s *me, uint8_t baseval, uint8_t maxval, uint8_t inc)
{
  uint8_t basecount = 0;
  uint8_t maxcount = 0;
  // Fix OOB conditions
  if (me->rgb[0] < baseval) me->rgb[0] = baseval;
  if (me->rgb[1] < baseval) me->rgb[1] = baseval;
  if (me->rgb[2] < baseval) me->rgb[2] = baseval;
  if (me->rgb[0] > maxval) me->rgb[0] = maxval;
  if (me->rgb[1] > maxval) me->rgb[1] = maxval;
  if (me->rgb[2] > maxval) me->rgb[2] = maxval;
  // Force valid set of base/max
  for (int i = 0; i < 3; i++)
  {
    if (me->rgb[i] == baseval) basecount++;
    if (me->rgb[i] == maxval) maxcount++;
  }

  if (maxcount > 1)
  {
    if (me->rgb[0] == maxval && me->rgb[1] == maxval)
      me->rgb[0] = SUB_FLOOR8(me->rgb[0], inc);
    if (me->rgb[1] == maxval && me->rgb[2] == maxval)
      me->rgb[1] = SUB_FLOOR8(me->rgb[1], inc);
    if (me->rgb[3] == maxval && me->rgb[1] == maxval)
      me->rgb[3] = SUB_FLOOR8(me->rgb[3], inc);
  }
  else if (basecount == 0 && maxcount == 0) {
    me->rgb[0] = maxval;
    me->rgb[1] = baseval;
    me->rgb[2] = baseval;
  }
  // Input validated.  Carry on
  if (me->rgb[0] >= maxval)
  {
    if (me->rgb[2] > baseval)
      me->rgb[2] = SUB_FLOOR8(me->rgb[2], inc);
    else
      me->rgb[1] = ADD_CEIL8(me->rgb[1], inc);
  }
  if (me->rgb[1] >= maxval)
  {
    if (me->rgb[0] > baseval)
      me->rgb[0] = SUB_FLOOR8(me->rgb[0], inc);
    else
      me->rgb[2] = ADD_CEIL8(me->rgb[2], inc);
  }
  if (me->rgb[2] >= maxval)
  {
    if (me->rgb[1] > baseval)
      me->rgb[1] = SUB_FLOOR8(me->rgb[1], inc);
    else
      me->rgb[0] = ADD_CEIL8(me->rgb[0], inc);
  }


}


//base = 0 ; max < 255 => dark rainbow
//base > 0 ; max = 255 => light rainbow
uint32_t makeRainbow(uint8_t baseval, uint8_t maxval, uint8_t inc, uint8_t start_led)
{
  //count up one byte, count up next byte, count down first byte, repeat + wrap for purple.
  uint8_t rgb[3]; //r, g, b
  rgb[0] = rgb[1] = rgb[2] = baseval;
  int current = 0;
  int next = 1;

  uint8_t led = start_led; //current array element


  while (rgb[current] < maxval) //bootstrap
  {
    rgb[current] = ADD_CEIL8(rgb[current], inc);
    strip.setPixelColor(led, rgb[0], rgb[1], rgb[2]);
    strip.show();
    led++;
    led %= NUM_LEDS;
  }

  do
  {

    while (rgb[next] < maxval)
    {
      rgb[next] = ADD_CEIL8(rgb[next], inc);
      strip.setPixelColor(led, rgb[0], rgb[1], rgb[2]);
      strip.show();
      led++;
      led %= NUM_LEDS;
    }


    while ((int16_t)rgb[current] > baseval)
    {
      rgb[current] = SUB_FLOOR8(rgb[current], inc);
      strip.setPixelColor(led, rgb[0], rgb[1], rgb[2]);
      strip.show();
      led++;
      led %= NUM_LEDS;
    }
    current = next;
    next = (current + 1) % 3;

  } while (current != 0); //while we aren't back to 0

  return led;
}


void wipeRainbow(uint8_t wait)
{
  colorWipe(255, 0, 0, wait);
  colorWipe(255, 255, 0, wait);

  colorWipe(0, 255, 0, wait);
  colorWipe(0, 255, 255, wait);

  colorWipe(0, 0, 255, wait);
  colorWipe(255, 0, 255, wait);
}
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void colorWipe(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, r, g, b);
    strip.show();
    delay(wait);
  }
}



