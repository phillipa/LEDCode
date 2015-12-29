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
CRGB bluejay=0x2B547E;
CRGB navyblue = 0x000080;
CRGB lapisblue = 0x15317E;
CRGB cornflowerblue=CRGB::CornflowerBlue;
CRGB earthblue = 0x0000A0;
CRGB cobaltblue = 0x0020C2;
CRGB blueorchid = 0x1F45FC;


//b&w
CRGB black  = CRGB::Black;
CRGB white = CRGB::White;

//basic set of colors, reasonably bright
CRGBPalette16 adbasic = CRGBPalette16(
                                   purepurple,  purepurple,  purepurple ,  purepurple,
                                   blue, blue, blue,  CRGB::Chartreuse,
                                   green,  purepurple,  purepurple,  purepurple,
                                   blue, blue, darkmagenta,  darkmagenta );

                                   
CRGBPalette16 abasic = CRGBPalette16( 0xca226b,0xca226b,0xca226b,0xca226b,
                                  0xf660ab,0xf660ab,0xf660ab,0xfff380,
                                  0xfff380,0xca226b,0xca226b,0xca226b,
                                  0xf660ab,0xf660ab,0xfff380,0xfff380);
                        
CRGBPalette16 purplegreen = CRGBPalette16(darkmagenta,darkorchid,darkviolet,mediumorchid,
                                          mediumpurple,purple,blueviolet,blue,
                                          blue,blue,chartreuse, green,
                                          chartreuse,yellowgreen, lime,chartreuse);

CRGBPalette16 greenpurple = CRGBPalette16(chartreuse,lime,yellowgreen,chartreuse,
                                          green,chartreuse,blue,blue,
                                          blue,blueviolet,purple, mediumpurple,
                                          mediumpurple,darkviolet, darkorchid,darkmagenta);                           
                            
CRGBPalette16 darkpurples = CRGBPalette16(
                                    darkmagenta, darkmagenta, darkorchid, darkorchid,
                                    darkviolet, darkviolet, blueviolet, blueviolet,
                                    darkmagenta, darkmagenta, darkorchid, darkorchid,
                                    darkviolet, darkviolet, blueviolet, blueviolet);

CRGBPalette16 blues = CRGBPalette16(
                                    blue, blue, bluejay, bluejay,
                                    navyblue, navyblue, lapisblue, lapisblue,
                                    cornflowerblue, cornflowerblue, earthblue, earthblue,
                                    cobaltblue, cobaltblue, blueorchid, blueorchid);
                                    
CRGBPalette16 greens = CRGBPalette16(
                                    limegreen, limegreen, lime, lime,
                                    green, green, chartreuse, chartreuse,
                                    yellowgreen, yellowgreen, chartreuse, chartreuse,
                                    limegreen, limegreen, lime, lime);
CRGBPalette16 yellows = CRGBPalette16(
                                  0xffff00, 0xffff00,  0xffdb58, 0xffdb58,
                                  0xffd801, 0xffd801, 0xfdd017, 0xfdd017,
                                  0xeac117, 0xeac117, 0xfbb917, 0xfbb917,
                                  0xfbb117, 0xfbb117, 0xe9ab17, 0xe9ab17);
CRGBPalette16 pinks = CRGBPalette16(
                                  0xf660ab, 0xf660ab, 0xf6358a, 0xf6358a,
                                  0xf52887, 0xf52887, 0xe45e9d, 0xe45e9d,
                                  0xe4287c, 0xe4287c, 0xf535aa, 0xf535aa,
                                  0xca226b, 0xca226b, 0xc12267, 0xc12267
                  );
CRGBPalette16 whites = CRGBPalette16 (
                0xf0ffff,0xf0ffff,0xffffc2, 0xffffc2,
                0xffffcc, 0xffffcc, 0xfbf6d9, 0xfbf6d9,
                0xfaebd7, 0xfaebd7, 0xfdd7e4, 0xfdd7e4,
                0xfcdff, 0xfcdff, 0xfdeef4, 0xfdeef4);


   
