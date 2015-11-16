/*** Hacking together a tokyoflash style clock
 *  on circle LEDS. just 'cause
 */

#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <stdint.h>
//#include <Wire.h>
#include <Time.h>
#include "palettes.h"
//#include "RTClib.h"
#include "agent.h"
#include "agent_clock.h"


// RTC_DS1307 RTC;

//other definitions
#define DATA_PIN     17
#define BRIGHTNESS  32 //took brightness down by 32 PG
#define UPDATES_PER_SECOND 100
#define BLENDING LINEARBLEND

#define CENTERLEDS 8
#define HOURLEDS 12
#define MINLEDS 16
#define SECLEDS 24
#define NUM_LEDS HOURLEDS+MINLEDS+SECLEDS+CENTERLEDS

CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette; //assign before palette color grabbing
TBlendType    currentBlending;

/** Agent Clock Variables **/

uint8_t clock_agents_here[NUM_LEDS];
//start with moody arlene + deki clock
AgentClock aclock(darkpurples, blues, greens, NUM_LEDS, clock_agents_here, BLENDING, BRIGHTNESS);
CRGBPalette16 bgpalette = adbasic;
uint8_t clocktype = 0; //goth clock = 0; arlene clock = 1


void setup() {

 // Wire.begin();
 // RTC.begin();
 // Serial.begin(9600);
  //Set up the LEDs
  //delay( 3000 ); // power-up safety delay
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );
  for (int i = 0; i < NUM_LEDS; i++) //make them all black
    leds[i] = CRGB::Pink;
  FastLED.show();
  delay(2000);//show red for 2 secs so i know it rebooted.



  //set up random numbers
  randomSeed(analogRead(0));
  randomSeed(analogRead(random(0, 7)));

  // put your setup code here, to run once:
  currentPalette = greens;
  currentBlending = LINEARBLEND;

}

void loop() {

  static uint8_t start_index = 0; //palette index
  // put your main code here, to run repeatedly:
//  plotFracTimeNoPalettes();
  
  //    FastLED.show();
    //  FastLED.delay(1000 / UPDATES_PER_SECOND);

                start_index++;
      aclock.drawagentclock(clock_agents_here, leds, 0, bgpalette, start_index);
      colorInnerRing(ColorFromPalette(adbasic, start_index += 1));
       FastLED.show(); 
       FastLED.delay(1000 / UPDATES_PER_SECOND);

}

void colorInnerRing(CRGB color)
{
  int start_idx = NUM_LEDS-CENTERLEDS;
  for (int i = 0; i < CENTERLEDS; i++)
  {
    leds[start_idx + i ] = color;
  }
}

void plotFracTime()
{
  int curr_index = 0;
  //  while (curr_index < (NUM_LEDS + 72)) //allow partial clock as long as whole clock isn't outside of strip
  // {
  //static DateTime  dt;
  
 
  uint8_t seconds = (second() * SECLEDS) / 60;
  uint8_t minutes = (minute() * MINLEDS) / 60 ;
  uint8_t hours = hour() >> 2; // hour is 0-11
  plotXofY(curr_index + 0, seconds, SECLEDS, blues, darkpurples);
  plotXofY(curr_index + SECLEDS, minutes, MINLEDS, greens, blues);
  plotXofY(curr_index + SECLEDS + MINLEDS, hours, HOURLEDS, darkpurples, greens);


  //}
}


void plotFracTimeNoPalettes()
{
  int curr_index = 0;
  //  while (curr_index < (NUM_LEDS + 72)) //allow partial clock as long as whole clock isn't outside of strip
  // {
  //static DateTime  dt;
  
 
  uint8_t seconds = (second() * SECLEDS) / 60;
  uint8_t minutes = (minute() * MINLEDS) / 60 ;
  uint8_t hours = hour() >> 2; // hour is 0-11
  plotXofY(curr_index + 0, seconds, SECLEDS, CRGB::Blue, purepurple);
  plotXofY(curr_index + SECLEDS, minutes, MINLEDS, CRGB::Chartreuse, CRGB::Blue);
  plotXofY(curr_index + SECLEDS + MINLEDS, hours, HOURLEDS, purepurple, CRGB::Green);


  //}
}
void plotXofY(int start_index, int num_on, int num_bits, CRGBPalette16 one, CRGBPalette16 zero)
{
  int pal_idx = 0;
  for (int i = 0; i < num_on; i++)
    leds[start_index + i] = ColorFromPalette(one, pal_idx += 25);

  pal_idx = 0;
  if (num_on < num_bits)
  {
    for (int i = num_on; i < num_bits; i++)
      leds[start_index + i] = ColorFromPalette(zero, pal_idx += 25);
  }
}

void plotXofY(int start_index, int num_on, int num_bits, CRGB one, CRGBPalette16 zero)
{
  for (int i = 0; i < num_on; i++)
    leds[start_index + i] = one;
  int pal_idx = 0;
  if (num_on < num_bits)
  {
    for (int i = num_on; i < num_bits; i++)
      leds[start_index + i] = ColorFromPalette(zero, pal_idx += 25);
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

