/*
  Base Project / @author 'Stream Cheap' Mini Macro Keyboard / David Madison
  @link partsnotincluded.com/electronics/diy-stream-deck-mini-macro-keyboard

  Added oled screen and support leds for retro illuminate by Nicolas ELIE / Twitter @nico_themaire
  https://github.com/themaire/StreamDeck

  2020/06 Added more icons for the rest of functions

  F14 = scène d'intro
  F15 = live scène
  F16 = pause Scène
  F17 = scène d'outro
  F18 = start/stop diffusion
  F19 = start/stop record
  F20 = mute micro/show micro

  2020/08 Erico Mendonca (doccaz): added default screen, changed order of buttons; simplified the code; converted LEDs into status indicators for 4 lower buttons.

*/

// ---------------------------------
// OLED Lib
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keyboard.h>
#include <arduino-timer.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

// Default wiring : SDA > GPIO2  --- SCK > GPIO3
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

auto timer = timer_create_default();

// show default display after 3 seconds of inactivity


// fade functions!
void fadeout() {
  for (int dim = 150; dim >= 0; dim -= 30) {
    display.ssd1306_command(0x81);
    display.ssd1306_command(dim); //max 157
    delay(50);
  }


  for (int dim2 = 34; dim2 >= 0; dim2 -= 17) {
    display.ssd1306_command(0xD9);
    display.ssd1306_command(dim2);  //max 34
    delay(100);
  }
}

void fadein() {
  for (int dim = 0; dim <= 160; dim += 30) {
    display.ssd1306_command(0x81);
    display.ssd1306_command(dim); //max 160
    delay(50);
  }


  for (int dim2 = 0; dim2 <= 34; dim2 += 17) {
    display.ssd1306_command(0xD9);
    display.ssd1306_command(dim2);  //max 34
    delay(100);
  }
}

//// Icons
#define Icon_width 32
#define Icon_height 32
static const unsigned char mute[128] PROGMEM =
{
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xc0, 0x00,
  0x00, 0x03, 0xc0, 0x00,
  0x00, 0x07, 0xc0, 0x00,
  0x00, 0x0f, 0xc0, 0x00,
  0x00, 0x1f, 0xc0, 0x00,
  0x00, 0x3f, 0xcc, 0x04,
  0x3f, 0xff, 0xce, 0x0e,
  0x7f, 0xff, 0xc6, 0x1c,
  0x7f, 0xff, 0xc7, 0x38,
  0x7f, 0xff, 0xc3, 0xf0,
  0x7f, 0xff, 0xc1, 0xe0,
  0x7f, 0xff, 0xc0, 0xe0,
  0x7f, 0xff, 0xc1, 0xf0,
  0x7f, 0xff, 0xc3, 0xb0,
  0x7f, 0xff, 0xc7, 0x18,
  0x3f, 0xff, 0xce, 0x1c,
  0x00, 0x7f, 0xcc, 0x0e,
  0x00, 0x3f, 0xc0, 0x00,
  0x00, 0x1f, 0xc0, 0x00,
  0x00, 0x07, 0xc0, 0x00,
  0x00, 0x03, 0xc0, 0x00,
  0x00, 0x01, 0xc0, 0x00,
  0x00, 0x00, 0x80, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};

static const unsigned char no_mute[128] PROGMEM =
{
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xc0, 0x00,
  0x00, 0x03, 0xc0, 0x00,
  0x00, 0x07, 0xc0, 0x00,
  0x00, 0x0f, 0xc0, 0x00,
  0x00, 0x1f, 0xc0, 0x00,
  0x00, 0x3f, 0xc0, 0x00,
  0x3f, 0xff, 0xc0, 0x00,
  0x7f, 0xff, 0xc0, 0x00,
  0x7f, 0xff, 0xc0, 0x00,
  0x7f, 0xff, 0xc0, 0x00,
  0x7f, 0xff, 0xc0, 0x00,
  0x7f, 0xff, 0xc0, 0x00,
  0x7f, 0xff, 0xc0, 0x00,
  0x7f, 0xff, 0xc0, 0x00,
  0x7f, 0xff, 0xc0, 0x00,
  0x3f, 0xff, 0xc0, 0x00,
  0x00, 0x7f, 0xc0, 0x00,
  0x00, 0x3f, 0xc0, 0x00,
  0x00, 0x1f, 0xc0, 0x00,
  0x00, 0x07, 0xc0, 0x00,
  0x00, 0x03, 0xc0, 0x00,
  0x00, 0x01, 0xc0, 0x00,
  0x00, 0x00, 0x80, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};
//ADDED ICONS
static const unsigned char emis[128] PROGMEM = {
  0x00, 0x01, 0xC0, 0x00,
  0x00, 0x01, 0xF8, 0x00,
  0x00, 0x00, 0x1E, 0x00,
  0x00, 0x00, 0x07, 0x80,
  0x00, 0x01, 0x01, 0xC0,
  0x00, 0x01, 0xE0, 0xE0,
  0x00, 0x00, 0x78, 0x70,
  0x00, 0x00, 0x0E, 0x30,
  0x00, 0x00, 0x07, 0x18,
  0x00, 0x01, 0xC3, 0x0C,
  0x00, 0x01, 0xE1, 0x8C,
  0x06, 0x00, 0x30, 0xC4,
  0x0F, 0x01, 0x98, 0xC4,
  0x1F, 0xC3, 0xC8, 0xC4,
  0x1F, 0xE7, 0xCC, 0xC4,
  0x1F, 0xF3, 0x80, 0x00,
  0x3F, 0xFC, 0x00, 0x00,
  0x3F, 0xFC, 0x00, 0x00,
  0x3F, 0xFE, 0x00, 0x00,
  0x1F, 0xFF, 0x00, 0x00,
  0x1F, 0xFF, 0x80, 0x00,
  0x1F, 0xFF, 0xC0, 0x00,
  0x0F, 0xFF, 0xE0, 0x00,
  0x0F, 0xFF, 0xF0, 0x00,
  0x07, 0xFF, 0xF0, 0x00,
  0x0D, 0xFF, 0xE0, 0x00,
  0x0F, 0x7F, 0x00, 0x00,
  0x1F, 0x80, 0x00, 0x00,
  0x1F, 0xC0, 0x00, 0x00,
  0x3F, 0xC0, 0x00, 0x00,
  0x3F, 0xE0, 0x00, 0x00,
  0x3F, 0xE0, 0x00, 0x00
};
const unsigned char no_emis[128] PROGMEM = {
  0x00, 0x01, 0xC0, 0x00,
  0x00, 0x01, 0xF8, 0x00,
  0x00, 0x00, 0x1E, 0x18,
  0x00, 0x00, 0x07, 0x3C,
  0x00, 0x00, 0x00, 0x7C,
  0x00, 0x01, 0xF0, 0xF8,
  0x00, 0x00, 0x79, 0xF0,
  0x00, 0x00, 0x03, 0xE0,
  0x00, 0x00, 0x07, 0xC8,
  0x00, 0x01, 0xCF, 0x8C,
  0x00, 0x01, 0xBF, 0x0C,
  0x06, 0x00, 0x7E, 0x4C,
  0x0F, 0x00, 0xFC, 0xC4,
  0x1F, 0xC1, 0xF8, 0xC4,
  0x1F, 0xE3, 0xF0, 0xC4,
  0x1F, 0xE7, 0xE0, 0x00,
  0x3F, 0xCF, 0xC0, 0x00,
  0x3F, 0x9F, 0x80, 0x00,
  0x3F, 0x3E, 0x00, 0x00,
  0x1E, 0x7C, 0x00, 0x00,
  0x1C, 0xF9, 0x80, 0x00,
  0x19, 0xF3, 0xC0, 0x00,
  0x03, 0xE7, 0xE0, 0x00,
  0x0F, 0xCF, 0xF0, 0x00,
  0x1F, 0x9F, 0xF0, 0x00,
  0x3F, 0x3F, 0xE0, 0x00,
  0x1E, 0x7F, 0x80, 0x00,
  0x0D, 0x80, 0x00, 0x00,
  0x03, 0xC0, 0x00, 0x00,
  0x17, 0xC0, 0x00, 0x00,
  0x3F, 0xE0, 0x00, 0x00,
  0x3F, 0xE0, 0x00, 0x00
};

const unsigned char pause[128] PROGMEM = {
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x07, 0xF0, 0x00,
  0x00, 0x3F, 0xFC, 0x00,
  0x00, 0xFF, 0xFF, 0x00,
  0x01, 0xF0, 0x0F, 0x80,
  0x03, 0xC0, 0x03, 0xC0,
  0x07, 0x00, 0x00, 0xE0,
  0x0F, 0x00, 0x00, 0x70,
  0x0E, 0x00, 0x00, 0x70,
  0x1C, 0x00, 0x00, 0x38,
  0x1C, 0x0C, 0x30, 0x18,
  0x18, 0x0C, 0x30, 0x1C,
  0x38, 0x0C, 0x30, 0x1C,
  0x38, 0x0C, 0x30, 0x1C,
  0x38, 0x0C, 0x30, 0x1C,
  0x38, 0x0C, 0x30, 0x1C,
  0x38, 0x0C, 0x30, 0x1C,
  0x38, 0x0C, 0x30, 0x1C,
  0x38, 0x0C, 0x30, 0x1C,
  0x18, 0x0C, 0x30, 0x18,
  0x1C, 0x00, 0x00, 0x38,
  0x0E, 0x00, 0x00, 0x38,
  0x0E, 0x00, 0x00, 0x70,
  0x07, 0x00, 0x00, 0xE0,
  0x03, 0x80, 0x01, 0xE0,
  0x01, 0xE0, 0x07, 0xC0,
  0x01, 0xF8, 0x1F, 0x80,
  0x00, 0x7F, 0xFE, 0x00,
  0x00, 0x1F, 0xF8, 0x00,
  0x00, 0x00, 0x80, 0x00,
  0x00, 0x00, 0x00, 0x00
};
const unsigned char other[128] PROGMEM = {
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x03, 0xC0, 0x00,
  0x00, 0x1F, 0xFC, 0x00,
  0x00, 0x7C, 0x1F, 0x00,
  0x01, 0xE0, 0x03, 0x80,
  0x03, 0x80, 0x00, 0xE0,
  0x06, 0x00, 0x00, 0x60,
  0x0C, 0x00, 0x00, 0x30,
  0x0C, 0x03, 0xC0, 0x18,
  0x18, 0x07, 0xE0, 0x0C,
  0x18, 0x07, 0xF0, 0x0C,
  0x30, 0x00, 0x70, 0x04,
  0x30, 0x00, 0x70, 0x06,
  0x30, 0x00, 0x70, 0x06,
  0x20, 0x00, 0xE0, 0x06,
  0x20, 0x01, 0xC0, 0x06,
  0x60, 0x01, 0xC0, 0x06,
  0x20, 0x03, 0x80, 0x06,
  0x20, 0x03, 0x80, 0x06,
  0x30, 0x01, 0x80, 0x06,
  0x30, 0x00, 0x00, 0x04,
  0x10, 0x03, 0x80, 0x0C,
  0x18, 0x03, 0x80, 0x0C,
  0x1C, 0x03, 0x80, 0x18,
  0x0C, 0x00, 0x00, 0x30,
  0x06, 0x00, 0x00, 0x70,
  0x03, 0x00, 0x00, 0xE0,
  0x01, 0xC0, 0x03, 0xC0,
  0x00, 0xF0, 0x0F, 0x00,
  0x00, 0x3F, 0xFC, 0x00,
  0x00, 0x07, 0xE0, 0x00,
  0x00, 0x00, 0x00, 0x00
};
static const unsigned char live[128] PROGMEM = {
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x3F, 0xFF, 0xFE, 0x00,
  0x60, 0x00, 0x03, 0x00,
  0x40, 0x00, 0x01, 0x07,
  0x40, 0x00, 0x01, 0x1F,
  0x40, 0x00, 0x01, 0x33,
  0x40, 0x00, 0x01, 0xE3,
  0x40, 0x00, 0x11, 0x83,
  0x44, 0x29, 0x70, 0x03,
  0x44, 0x2D, 0x40, 0x03,
  0x44, 0x2F, 0x40, 0x03,
  0x44, 0x27, 0x78, 0x03,
  0x44, 0x26, 0x70, 0x03,
  0x44, 0x26, 0x40, 0x03,
  0x44, 0x26, 0x70, 0x03,
  0x47, 0xA6, 0x79, 0x03,
  0x40, 0x00, 0x01, 0xC3,
  0x40, 0x00, 0x01, 0xE3,
  0x40, 0x00, 0x01, 0x73,
  0x40, 0x00, 0x01, 0x1F,
  0x40, 0x00, 0x01, 0x0F,
  0x60, 0x00, 0x03, 0x02,
  0x3F, 0xFF, 0xFE, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};
const unsigned char intro[128] PROGMEM = {
  0x80, 0x00, 0x00, 0x01,
  0x80, 0x00, 0x40, 0x00,
  0x00, 0x00, 0x70, 0x00,
  0x00, 0x00, 0x7E, 0x00,
  0x00, 0x00, 0x07, 0x00,
  0x00, 0x00, 0x41, 0xC0,
  0x00, 0x00, 0x78, 0xE0,
  0x00, 0x00, 0x04, 0x60,
  0x00, 0x00, 0x02, 0x30,
  0x00, 0x03, 0x81, 0x18,
  0x00, 0x03, 0xC0, 0x98,
  0x00, 0x01, 0xC0, 0x48,
  0x00, 0x00, 0xC0, 0x4C,
  0x00, 0x00, 0xC0, 0x4C,
  0x00, 0x00, 0xC0, 0x0C,
  0x00, 0x00, 0xC0, 0x00,
  0x00, 0x60, 0xC0, 0x50,
  0x00, 0x00, 0xC0, 0x6C,
  0x00, 0x00, 0xC0, 0x4C,
  0x00, 0x08, 0xC0, 0x4C,
  0x00, 0x10, 0xC0, 0x08,
  0x00, 0x20, 0xC0, 0x98,
  0x00, 0x40, 0x01, 0x18,
  0x00, 0xC0, 0x02, 0x30,
  0x01, 0x20, 0x04, 0x60,
  0x03, 0x0E, 0x70, 0xC0,
  0x07, 0x80, 0x41, 0x80,
  0x08, 0xE0, 0x07, 0x00,
  0x10, 0x7E, 0x7E, 0x00,
  0x20, 0x0E, 0x70, 0x00,
  0xC0, 0x00, 0x00, 0x00,
  0xC0, 0x00, 0x00, 0x03
};
const unsigned char alttab[128] PROGMEM = {
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x0F, 0xFF, 0xFF, 0x00,
  0x1F, 0xFF, 0xFF, 0x80,
  0x30, 0x00, 0x00, 0x80,
  0x30, 0x00, 0x00, 0xC0,
  0x30, 0x00, 0x00, 0xF0,
  0x30, 0x00, 0x00, 0xF8,
  0x30, 0x00, 0x00, 0xC8,
  0x30, 0x00, 0x00, 0xCC,
  0x30, 0x00, 0x00, 0xCC,
  0x30, 0x00, 0x00, 0xCC,
  0x30, 0x00, 0x00, 0xCC,
  0x30, 0x00, 0x00, 0xCC,
  0x30, 0x00, 0x00, 0xCC,
  0x30, 0x00, 0x00, 0xCC,
  0x30, 0x00, 0x00, 0xCC,
  0x30, 0x00, 0x00, 0xCC,
  0x30, 0x00, 0x00, 0xCC,
  0x30, 0x00, 0x00, 0xCC,
  0x30, 0x00, 0x00, 0xCC,
  0x30, 0x00, 0x00, 0xCC,
  0x30, 0x00, 0x00, 0xCC,
  0x1C, 0x00, 0x03, 0xCC,
  0x1F, 0xFF, 0xFF, 0x8C,
  0x03, 0xFF, 0xFF, 0x0C,
  0x03, 0x00, 0x00, 0x0C,
  0x03, 0x80, 0x00, 0x18,
  0x01, 0xFF, 0xFF, 0xF8,
  0x00, 0x7F, 0xFF, 0xE0,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};
const unsigned char play[128] PROGMEM = {
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xE0, 0x00,
  0x00, 0x1F, 0xFE, 0x00,
  0x00, 0x7F, 0xFF, 0x80,
  0x00, 0xF8, 0x07, 0xC0,
  0x01, 0xE0, 0x01, 0xE0,
  0x03, 0xC0, 0x00, 0xF0,
  0x07, 0x80, 0x00, 0x70,
  0x07, 0x00, 0x00, 0x38,
  0x0E, 0x00, 0x00, 0x18,
  0x0E, 0x06, 0x00, 0x1C,
  0x0C, 0x07, 0x80, 0x0C,
  0x1C, 0x07, 0xE0, 0x0C,
  0x1C, 0x07, 0xF0, 0x0E,
  0x1C, 0x07, 0xF8, 0x0E,
  0x1C, 0x07, 0xF8, 0x0E,
  0x1C, 0x07, 0xE0, 0x0C,
  0x1C, 0x07, 0xC0, 0x0C,
  0x0C, 0x07, 0x00, 0x0C,
  0x0E, 0x00, 0x00, 0x1C,
  0x0E, 0x00, 0x00, 0x18,
  0x07, 0x00, 0x00, 0x38,
  0x03, 0x80, 0x00, 0x70,
  0x03, 0xC0, 0x00, 0xF0,
  0x01, 0xE0, 0x01, 0xE0,
  0x00, 0xFC, 0x0F, 0x80,
  0x00, 0x3F, 0xFF, 0x00,
  0x00, 0x0F, 0xFC, 0x00,
  0x00, 0x01, 0xE0, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};
//END OF ADDED ICONS
static const unsigned char rec[128] PROGMEM =
{
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x07, 0xf0, 0x00,
  0x00, 0x3f, 0xfc, 0x00,
  0x00, 0x7f, 0xfe, 0x00,
  0x01, 0xff, 0xff, 0x80,
  0x03, 0xff, 0xff, 0xc0,
  0x03, 0xff, 0xff, 0xc0,
  0x07, 0xff, 0xff, 0xe0,
  0x0f, 0xff, 0xff, 0xf0,
  0x0f, 0xff, 0xff, 0xf0,
  0x0f, 0xff, 0xff, 0xf8,
  0x1f, 0xff, 0xff, 0xf8,
  0x1f, 0xff, 0xff, 0xf8,
  0x1f, 0xff, 0xff, 0xf8,
  0x1f, 0xff, 0xff, 0xf8,
  0x1f, 0xff, 0xff, 0xf8,
  0x1f, 0xff, 0xff, 0xf8,
  0x1f, 0xff, 0xff, 0xf8,
  0x0f, 0xff, 0xff, 0xf0,
  0x0f, 0xff, 0xff, 0xf0,
  0x07, 0xff, 0xff, 0xe0,
  0x03, 0xff, 0xff, 0xc0,
  0x03, 0xff, 0xff, 0xc0,
  0x01, 0xff, 0xff, 0x80,
  0x00, 0x7f, 0xfe, 0x00,
  0x00, 0x3f, 0xfc, 0x00,
  0x00, 0x0f, 0xf0, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};

static const unsigned char swico[128] PROGMEM =
{
  0xff, 0xff
};


// UVLED GPIO definition
const uint8_t UVLED_1 = 9;
const uint8_t UVLED_2 = 8;
const uint8_t UVLED_3 = 16;
const uint8_t UVLED_4 = 10;

class uvLed {
  public:
    uvLed(uint8_t pin) {
      pinLed = pin;
      init();
    }

    void init() {
      pinMode(pinLed, OUTPUT);
      digitalWrite(pinLed, LOW);
    }

    void on() {
      digitalWrite(pinLed, HIGH);
    }

    void analOn(int duty) {
      analogWrite(pinLed, duty);
    }

    void off() {
      digitalWrite(pinLed, LOW);
    }

  private:
    uint8_t pinLed;
};

//UV leds objects array
uvLed uvLeds[] = {
  {UVLED_1},
  {UVLED_2},
  {UVLED_3},
  {UVLED_4}
};

const uint8_t NumUvLeds = sizeof(uvLeds) / sizeof(uvLed);

// Key definitions
const uint8_t BUTTON_KEY1 = KEY_F14;
const uint8_t BUTTON_KEY2 = KEY_F15;
const uint8_t BUTTON_KEY3 = KEY_LEFT_ALT;
const uint8_t BUTTON_KEY5 = KEY_F16;
const uint8_t BUTTON_KEY4 = KEY_F17;
const uint8_t BUTTON_TAB = KEY_TAB;
const uint8_t BUTTON_KEY6 = KEY_F18;
const uint8_t BUTTON_KEY7 = KEY_F19;
const uint8_t BUTTON_KEY8 = KEY_F20;

// Btn GPIO definitions
const uint8_t BUTTON_PIN1 = A2;
const uint8_t BUTTON_PIN2 = A1;
const uint8_t BUTTON_PIN3 = A0;
const uint8_t BUTTON_PIN4 = 15;
const uint8_t BUTTON_PIN5 = 4;
const uint8_t BUTTON_PIN6 = 5;
const uint8_t BUTTON_PIN7 = 6;
const uint8_t BUTTON_PIN8 = 7;

const uint8_t LED_BTN5 = 0;
const uint8_t LED_BTN6 = 0;
const uint8_t LED_BTN7 = 0;
const uint8_t LED_BTN8 = 0;

const long debounceTime = 130;
unsigned long lastPressed;
uint8_t cptBtn = 0;

// Button helper class for handling press/release and debouncing
class button {
  public:
    const char key;
    const uint8_t pin;
    bool activated = false;
    uint8_t mode = 0;
    bool tabed = false;

    button(uint8_t k, uint8_t p) : key(k), pin(p) {}

    uint8_t setled(void) { // Prepare the led associated

      uint8_t pinled = -1;
      switch (pin) {
        case BUTTON_PIN8: // If pin is the mute btn
          // Create LED. Active low.
          pinled = LED_BTN8 ;
          pinMode(pinled, OUTPUT);
          digitalWrite(pinled, LOW);
          return pinled;
          break;

        case BUTTON_PIN7: // If pin is the record btn
          // Create LED. Active low.
          pinled = LED_BTN7 ;
          pinMode(pinled, OUTPUT);
          digitalWrite(pinled, LOW);
          return pinled;
          break;

        case BUTTON_PIN6: // If pin is the broadcast btn
          // Create LED. Active low.
          pinled = LED_BTN6 ;
          pinMode(pinled, OUTPUT);
          digitalWrite(pinled, LOW);
          return pinled;
          break;

        case BUTTON_PIN5: // If pin is the pause btn
          // Create LED. Active low.
          pinled = LED_BTN5 ;
          pinMode(pinled, OUTPUT);
          digitalWrite(pinled, LOW);
          return pinled;
          break;

        default:
          return 0;
          break;
      }

    }
    const uint8_t buttonled = setled();

    void clearBlank() {
      display.clearDisplay();
      display.display();
    }

    void printText(String myinputstring, int8_t x = 0, int8_t y = 0, int8_t siZe = 2) {
      display.clearDisplay();

      display.setTextSize(siZe); // Draw X-scale text
      display.setTextColor(WHITE);
      display.setCursor(x, y);
      display.println(myinputstring);
      display.display(); // Show expected text
    }

    bool press(boolean state) {

      if (state == pressed || (millis() - lastPressed  <= debounceTime)) {
        return; // Nothing to see here, folks
      }
      lastPressed = millis();

      state ? Keyboard.press(key) : Keyboard.release(key);
      pressed = state;


      if (pin == BUTTON_PIN3 and pressed) {
        if (!tabed) {
          Keyboard.press(BUTTON_TAB);
          delay(50);
          //Keyboard.release(BUTTON_TAB);
          Keyboard.releaseAll(); // For releasing Ctrl button too
          tabed = true;
        }
        tabed = false;
        Serial.println("ALT");
      }

      cptBtn += 1;

      if (pressed == 0) {

        if (pin == BUTTON_PIN1) {         //Intro Button code
          display.stopscroll();
          printText("  Intro", 34, 10);
          display.drawBitmap(1, 1, intro, 32, 32, 1);
          display.display();
        } else if (pin == BUTTON_PIN2) {         //Live scene Button code
          display.stopscroll();
          display.clearDisplay();
          display.setCursor(37, 16);
          display.print("Scene");
          display.drawBitmap(0, 0, live, 32, 32, 1);
          display.display();
        } else if (pin == BUTTON_PIN3) {         // ALT-TAB code
          display.stopscroll();
          display.clearDisplay();
          display.setCursor(37, 16);
          display.print("Alt-Tab");
          display.drawBitmap(0, 0, alttab, 32, 32, 1);
          display.display();
        } else if (pin == BUTTON_PIN4) {         // Other code
          display.stopscroll();
          display.clearDisplay();
          display.setCursor(37, 16);
          display.print("Other");
          display.drawBitmap(0, 0, other, 32, 32, 1);
          display.display();
        } else if (pin == BUTTON_PIN5) {         //Pause Button code
          if (activated == false) {
            activated = true;
            digitalWrite(UVLED_1, HIGH);
            printText("Pause", 40, 10);
            display.drawBitmap(1, 1, pause, 32, 32, 1);
            display.display();
          } else if (activated == true and cptBtn == 2) {
            activated = false;
            digitalWrite(UVLED_1, LOW);
            display.stopscroll();
            printText("Unpause", 33, 10);
            display.drawBitmap(0, 0, play, 32, 32, 1);
            display.display();
          }
        } else if (pin == BUTTON_PIN6) {         //Broadcast Button code
          // DIFFUSION
          display.stopscroll();
          if (activated == false) {
            activated = true;
            digitalWrite(UVLED_2, HIGH);
            printText("Bcast..", 36, 16);
            display.drawBitmap(1, 1, emis, 32, 32, 1);
            display.display();
          } else if (activated == true and cptBtn == 2) {
            activated = false;
            digitalWrite(UVLED_2, LOW);
            display.stopscroll();
            display.clearDisplay();
            display.setCursor(35, 0);
            display.print("No");
            display.setCursor(36, 16);
            display.print("Bcast..");
            display.drawBitmap(1, 1, no_emis, 32, 32, 1);
            display.display();
          }
        } else if (pin == BUTTON_PIN7) {         //Recording Button code
          // RECORDING
          if (activated == false) {
            activated = true;
            digitalWrite(UVLED_3, HIGH);
            printText("RECORD", 40, 10);
            display.drawBitmap(0, 0, rec, Icon_width, Icon_height, 1);
            display.display();
          } else if (activated == true and cptBtn == 2) {
            activated = false;
            digitalWrite(UVLED_3, LOW);
            display.stopscroll();
            printText("   STOP", 10, 10);
            display.fillRect(2, 2, 26, 26, WHITE);
            display.display();
          }
        } else if (pin == BUTTON_PIN8) {         //Mute Button code
          // MUTING
          if (activated == false) {
            activated = true;
            digitalWrite(UVLED_4, HIGH);
            printText("Shhh", 52, 10);
            display.drawBitmap(0, 0, mute, Icon_width, Icon_height, 1);
            display.startscrollright(0x00, 0x0F);
            display.display();
          } else if (activated == true and cptBtn == 2) {
            activated = false;
            digitalWrite(UVLED_4, LOW);
            display.stopscroll();
            printText("On AIR", 42, 10);
            display.drawBitmap(0, 0, no_mute, Icon_width, Icon_height, 1);
            display.display();
          }
        } else {
          clearBlank();
          // Draw pin n° with the simple screen function
          String charpin;
          charpin = String(pin) + " - " + String(buttonled) + "\n" + String(activated) + "- cptBtn: " + String(cptBtn);
          printText(charpin);
        }

        cptBtn = 0;
      }
      return state;
    }

    bool update() {
      return press(!digitalRead(pin));
    }

  private:
    const long debounceTime = 30;
    unsigned long lastPressed;
    boolean pressed = 0;
    uint8_t cptBtn = 0;
};

// Objects creations
// Button objects, organized in array
button buttons[] = {
  {BUTTON_KEY1, BUTTON_PIN1},
  {BUTTON_KEY2, BUTTON_PIN2},
  {BUTTON_KEY3, BUTTON_PIN3},
  {BUTTON_KEY4, BUTTON_PIN4},
  {BUTTON_KEY5, BUTTON_PIN5},
  {BUTTON_KEY6, BUTTON_PIN6},
  {BUTTON_KEY7, BUTTON_PIN7},
  {BUTTON_KEY8, BUTTON_PIN8},
};

const uint8_t NumButtons = sizeof(buttons) / sizeof(button);

// --------------------------------- End keyboard definition

bool showDefaultScreen() {
  // by default display the mute status
  button btn = buttons[7]; // mute button
  fadeout();
  if (!btn.activated) {
    btn.printText("On AIR", 42, 10);
    display.drawBitmap(0, 0, no_mute, Icon_width, Icon_height, 1);
    display.startscrollright(0x00, 0xFF);
  } else {
    btn.printText("Shhh", 52, 10);
    display.drawBitmap(0, 0, mute, Icon_width, Icon_height, 1);
    display.startscrollright(0x00, 0xFF);
  }
  display.display();
  fadein();

  // returns false or true depending on the parameter
  // true = repeat task at the end
  // false = do not repeat task at the end

  return true;
}

void failsafe() {
  for (;;) {} // Just going to hang out here for awhile :D
}

void ledsOn() {
  for (int i = 0; i < NumUvLeds; i++) {
    uvLeds[i].on();
  }
}

void ledsOff() {
  for (int i = 0; i < NumUvLeds; i++) {
    uvLeds[i].off();
  }
}

void setup() {
  //------ Startup SCREEN
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  // Clear the buffer
  display.clearDisplay();
  display.display();

  //------ Startup buttons
  // Safety check. Ground pin #1 (RX) to cancel keyboard inputs.
  pinMode(1, INPUT_PULLUP);
  if (!digitalRead(1)) {
    failsafe();
  }
  // Set pinmode for all button in the array
  for (int i = 0; i < NumButtons; i++) {
    pinMode(buttons[i].pin, INPUT_PULLUP);
  }

  // show default screen on startup
  showDefaultScreen();
}

void loop() {
  for (int i = 0; i < NumButtons; i++) {
    if (buttons[i].update()) {
      // start a timer in 8 seconds after the last keypress
      timer.cancel();
      auto task = timer.in(8000, showDefaultScreen);
    }
  }

  timer.tick(); // update the timer
  //delay(100);
}
