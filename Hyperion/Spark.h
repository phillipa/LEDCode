#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <stdint.h>

class Spark 
{

public:
     Spark();
     void initspark(CRGB p_color, uint16_t p_pos, uint8_t startfade, uint8_t fadeval);
     void reinit(CRGB p_color, uint16_t p_pos, uint8_t startfade, uint8_t fadeval);

     void fade();
     CRGB color; //color of agent
     CRGB initcolor; //what the color was initially.
     int16_t pos; // position of agent
     int8_t fadeval; //amount to fade by each turn
     boolean alive;
     
 

  
};

