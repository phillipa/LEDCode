#include "FatAgent.h"

#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <stdint.h>

#define C_BLACK CRGB::Black

#define _up 0
#define _down 1
#define _left 2
#define _right 3

//empty default constructor
FatAgent::FatAgent()
{
  
}

void FatAgent::initagent(CRGB p_color, uint16_t p_x, uint16_t p_y, uint8_t p_max_wait)
{
  color = p_color;
  x = p_x;
  y=p_y;

  //set random direction
  dir = 0;//random(0,3); //randomize directions
 
    
  pal = CRGBPalette16( p_color, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK );
  max_wait = p_max_wait;
  waited = 0;
  span = 8;
}

void FatAgent::setcolor(CRGB p_color)
{
    color = p_color;
    pal = CRGBPalette16( color, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK );

}


void FatAgent::setspan(uint8_t new_span)
{
   if(!span) return;  // Span of 0 is nonsensical
   span = new_span;
}

void FatAgent::draw(CRGB * LED_array, int row_width, int num_rows, uint16_t num_leds)
{
   // Start at agent position, proceed in the direction opposite dir until span has been reached.
   uint16_t min_pos, max_pos, cur_pos; 
   uint8_t pal_index;
   
   if(dir == _up)
   {
      for(int i = 0; i < span; i++)
      {
        if(y-i >= 0)
        {
          LED_array[x+((y-i)*row_width)] = ColorFromPalette(pal, 3*i);
        }
      }
   }
   else if(dir==_down)
   {
      for(int i = 0; i < span; i++)
      {
        if(y+i < num_rows)
        {
          LED_array[x+((y+i)*row_width)] = ColorFromPalette(pal, 3*i);
        }
      }
   }
   else if(dir == _left)
   {
      for(int i = 0; i < span; i++)
      {
        if(x+i < row_width)
        {
          LED_array[(x+i)+(y*row_width)] = ColorFromPalette(pal, 3*i);
        }
      }
   }        
   else if(dir== _right)
   {
      for(int i = 0; i < span; i++)
      {
        if(x-i >= 0)
        {
          LED_array[(x-i)+((y)*row_width)] = ColorFromPalette(pal, 3*i);
        }
      }
   }

  
}

/**
 * Have the agents move based on their wait time and direction
 */
void FatAgent::move(uint8_t agents_here[], int row_width, int num_rows, uint16_t num_leds)
{
  if (waited == max_wait)
  {
   if(dir == _up)
      y++;
   else if(dir == _down)
      y--;    
    else if(dir == _left)
      x--;
    else if(dir == _right)
      x++;

   if(x<0)
    x=row_width-1;
   else if(x>= row_width)
    x=0;
   if(y>= num_rows)
    y=0;
   else if(y < 0)
    y=num_rows;
    waited = 0;
  }
  else
    waited++;
}



