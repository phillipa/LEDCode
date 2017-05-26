#include "spark.h"

#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <stdint.h>

//empty default constructor
Spark::Spark()
{
  
}

void Spark::initspark(CRGB p_color, uint16_t p_pos, uint8_t startfade, uint8_t p_fadeval)
{
  
      color = p_color;
      initcolor = p_color;
      pos = p_pos;
      fadeval = p_fadeval;
      
      for(int i = 0; i < startfade; i++)
        fade();
     
}

void Spark::reinit(CRGB p_color, uint16_t p_pos, uint8_t startfade, uint8_t p_fadeval)
{
  
      color = p_color;
      initcolor = p_color;
      pos = p_pos;
      fadeval = p_fadeval;
      
      for(int i = 0; i < startfade; i++)
        fade();
     
}

/*void Spark::initspark()
{
  alive = false;
}
*/

void Spark::fade()
{
  if(color.r > fadeval)
    color.r -= fadeval;
  else
    color.r = 0;
  if(color.g > fadeval)
    color.g -= fadeval;
  else
    color.g = 0;
 if(color.b > fadeval)
    color.b -= fadeval;
  else
    color.b = 0;

  if(color.r < 0)
    color.r=0;
  if(color.g < 0)
    color.g=0;
  if(color.b < 0)
    color.b = 0;
    
}


