#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <TFT_eSPI.h>

#ifndef TFT_BACKLIGHT
#define TFT_BACKLIGHT 5
#endif

#define RGBto565(r, g, b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3))

class TFT : public TFT_eSPI {
 public:
  TFT() : TFT_eSPI() {}
  void init() {
    TFT_eSPI::init();
    pinMode(TFT_BACKLIGHT, OUTPUT);
  }
  void setBacklight(uint8_t brightness) {
    analogWrite(TFT_BACKLIGHT, 255 - brightness);
  }
};

extern TFT Display;  // Declare Display as extern

#endif  // DISPLAY_H