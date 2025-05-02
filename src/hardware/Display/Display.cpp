#include "Display.h"

display::display() : TFT_eSPI() {}

void display::init() {
  TFT_eSPI::init();
  pinMode(TFT_BACKLIGHT, OUTPUT);
}

void display::setBacklight(uint8_t brightness) {
#if TFT_BACKLIGHT_INVERTED
  analogWrite(TFT_BACKLIGHT, 255 - brightness);
#else
  analogWrite(TFT_BACKLIGHT, brightness);
#endif
}

display Display;  // Define Display
