#ifndef BUTTON_H
#define BUTTON_H

#include <OneButton.h>

#ifndef TOUCH_PIN
#define TOUCH_PIN 16
#endif
/**
 * @class button
 * @brief Button class derived from OneButton to manage touch sensor input.
 * @details This class provides functionality for handling button presses,
 *          including single, double, and long presses.
 * @note The default pin for the touch sensor is defined as TOUCH_PIN (GPIO 16).
 * @ingroup Hardware
 */
class button : public OneButton {
 public:
  button(uint8_t pin = TOUCH_PIN, bool activeLow = true, bool pullupActive = true)
      : OneButton(pin, activeLow, pullupActive) {}
};

extern button Button;

#endif  // BUTTON_H