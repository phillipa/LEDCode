#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <stdint.h>


//purples
CRGB darkmagenta = CRGB::DarkMagenta;
CRGB darkorchid = CRGB::DarkOrchid;
CRGB darkviolet = CRGB::DarkViolet;
CRGB fuchsia = CRGB::Fuchsia;
CRGB mediumorchid = CRGB::MediumOrchid;
CRGB mediumpurple = CRGB::MediumPurple;
CRGB purple = CRGB::Purple;
CRGB purepurple = CHSV( HUE_PURPLE, 255, 255);
CRGB blueviolet = CRGB::BlueViolet;
//greens
CRGB chartreuse = CRGB::Chartreuse;
CRGB yellowgreen = CRGB::YellowGreen;
CRGB green  = CHSV( HUE_GREEN, 255, 255);
CRGB lime = CRGB::Lime;
CRGB limegreen = CRGB::LimeGreen;
//blues
CRGB blue = CHSV( HUE_BLUE, 255,255);
//b&w
CRGB black  = CRGB::Black;
CRGB white = CRGB::White;

//basic set of colors, reasonably bright
CRGBPalette16 adbasic = CRGBPalette16(
                                   purepurple,  purepurple,  purepurple ,  purepurple,
                                   blue, blue, blue,  green,
                                   green,  purepurple,  purepurple,  purepurple,
                                   blue, blue, green,  green );

CRGBPalette16 purples = CRGBPalette16(
                                    darkmagenta, darkmagenta, darkorchid, darkorchid,
                                    darkviolet, darkviolet, fuchsia, fuchsia,
                                    mediumorchid, mediumorchid, mediumpurple, mediumpurple,
                                    purple, purple, purepurple, purepurple);
CRGBPalette16 darkpurples = CRGBPalette16(
                                    darkmagenta, darkmagenta, darkorchid, darkorchid,
                                    darkviolet, darkviolet, blueviolet, blueviolet,
                                    darkmagenta, darkmagenta, darkorchid, darkorchid,
                                    darkviolet, darkviolet, blueviolet, blueviolet);


CRGBPalette16 greens = CRGBPalette16(
                                    limegreen, limegreen, lime, lime,
                                    green, green, chartreuse, chartreuse,
                                    yellowgreen, yellowgreen, chartreuse, chartreuse,
                                    limegreen, limegreen, lime, lime);
                                    
