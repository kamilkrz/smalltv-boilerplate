
#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <TFT_eSPI.h>

#ifndef TFT_BACKLIGHT
#define TFT_BACKLIGHT 5
#endif

#ifndef TFT_BACKLIGHT_INVERTED
#define TFT_BACKLIGHT_INVERTED true
#endif

#define RGBto565(r, g, b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3))

/**
 * @class display
 * @brief Display class derived from TFT_eSPI to manage the display.
 * @ingroup Hardware
 */
class display : public TFT_eSPI {
 public:
  /**
   * @brief Constructor for the TFT class.
   */
  display();

  /**
   * @brief Initialize the display and configure the backlight pin.
   */
  void init();

  /**
   * @brief Set the backlight brightness.
   * @param brightness Brightness level (0-255).
   */
  void setBacklight(uint8_t brightness);

  void drawSplashScreen(const String& text, const String& secondLine) {
    fillScreen(TFT_BLACK);
    setTextColor(TFT_WHITE);
    setTextFont(4);
    setTextDatum(MC_DATUM);
    drawString(text, width() / 2, height() / 2);
    setTextFont(2);
    drawString(secondLine, width() / 2, height() / 2 + fontHeight(4) + 5);
    setTextDatum(TL_DATUM);
  }
};

extern display Display;  // Declare Display as extern

#endif  // DISPLAY_H
