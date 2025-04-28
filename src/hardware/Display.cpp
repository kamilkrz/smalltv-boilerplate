#include "Display.h"

TFT::TFT() : TFT_eSPI() {}

void TFT::init() {
  TFT_eSPI::init();
  pinMode(TFT_BACKLIGHT, OUTPUT);
}

void TFT::setBacklight(uint8_t brightness) {
#if TFT_BACKLIGHT_INVERTED
  analogWrite(TFT_BACKLIGHT, 255 - brightness);
#else
  analogWrite(TFT_BACKLIGHT, brightness);
#endif
}

TFT Display;  // Define Display
