#include "GooglyEyes.h"

#include "hardware/Button.h"
#include "hardware/Display.h"

googlyEyes& googlyEyes::getInstance() {
  static googlyEyes instance;
  return instance;
}

googlyEyes::googlyEyes() : App("Googly Eyes") {}

googlyEyes::~googlyEyes() = default;

void googlyEyes::init() {
  shouldExitApp = false;
  Display.fillScreen(RGBto565(0, 60, 60));
  Button.attachClick([]() {});
  Button.attachLongPressStart([]() {
    googlyEyes::getInstance().shouldExitApp = true;
  });
}

void googlyEyes::update() {
  Button.tick();  // Process button events
}

void googlyEyes::render() {
  static unsigned long lastDrawTime = 0;
  unsigned long currentTime = millis();
  if (currentTime - lastDrawTime >= 300) {
    lastDrawTime = currentTime;

    int w = Display.width();
    int w4 = w / 4;
    int h = Display.height();
    int h4 = h / 4;
    int xl = random(w4 - 10, w4 + 10);
    int xr = random(w4 * 3 - 10, w4 * 3 + 10);
    int yl = random(h4 * 2 - 10, h4 * 2 + 10);
    int yr = random(h4 * 2 - 10, h4 * 2 + 10);
    Display.fillEllipse(w4, h4 * 2, w4 - 10, h4, TFT_WHITE);
    Display.fillEllipse(w4 * 3, h4 * 2, w4 - 10, h4, TFT_WHITE);
    Display.fillCircle(xl, yl, 15, TFT_BLACK);
    Display.fillCircle(xr, yr, 15, TFT_BLACK);
  }
}

bool googlyEyes::shouldExit() {
  return shouldExitApp;
}
