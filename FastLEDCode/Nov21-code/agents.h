#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <stdint.h>

/* Different types of agent */
#define AGENT_DISABLED 0
#define AGENT_CLASSIC 1
#define AGENT_COMET 2

/*wandering agents*/
struct agent_s {
  uint8_t agent_type;
  CRGB color1; // Primary RGB color
  CRGB color2; // Secondary RGB color
  CRGBPalette16 palette;//palette (if using)
  uint8_t col_index; // where in the palette we are. 
  int16_t pos; // Position (literal or seed)
  uint8_t span; // Width or pixel count
  int8_t dir; // Direction (signed)
  uint16_t waited; // State counter for wait
  uint16_t max_wait;  // Max wait
};

void init_agent(struct agent_s* toinit, CRGB color1, int16_t posi, uint8_t dir, uint8_t maxw, void *fptr);
void init_agent(struct agent_s* toinit, CRGBPalette16 palette, uint8_t col_index, int16_t posi, uint8_t dir, uint8_t maxw, void *fptr);

void update_agent_colors();
void move_agents();
void move_agent(struct agent_s* toupdate);
void draw_agents();

void reverse_agent_dir(struct agent_s* toset);
void set_agent_dir(struct agent_s* toset, int8_t dir);
void set_agent_type(struct agent_s* toset, uint8_t atype);
void set_agent_span(struct agent_s* toset, uint8_t span);
void set_agent_pos(struct agent_s* toset, int16_t pos);

boolean sameDirection(struct agent_s* a, struct agent_s* b);

