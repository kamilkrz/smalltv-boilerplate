#include "GooglyEyes.h"

#include "hardware/Hardware.h"

GooglyEyesApp& GooglyEyesApp::getInstance() {
  static GooglyEyesApp instance;
  return instance;
}

GooglyEyesApp::GooglyEyesApp() : App("Googly Eyes") {}

GooglyEyesApp::~GooglyEyesApp() = default;

void GooglyEyesApp::init() {
  shouldExitApp = false;
  Display.fillScreen(RGBto565(0, 60, 60));
  // Draw static eyeballs once
  int w = Display.width();
  int w4 = w / 4;
  int h = Display.height();
  int h4 = h / 4;
  int rx = w4 - 10;
  int ry = h4;
  Display.fillEllipse(w4, h4 * 2, rx, ry, TFT_WHITE);       // Left eyeball
  Display.fillEllipse(w4 * 3, h4 * 2, rx, ry, TFT_WHITE);   // Right eyeball

  // Store geometry for later use
  this->w4 = w4;
  this->h4 = h4;
  this->rx = rx;
  this->ry = ry;
  this->pupilRadius = 15;
  this->rx_safe = rx - pupilRadius;
  this->ry_safe = ry - pupilRadius;

  // Initialize previous pupil positions to center
  prev_xl = w4;
  prev_yl = h4 * 2;
  prev_xr = w4 * 3;
  prev_yr = h4 * 2;

  Button.attachClick([]() {});
  Button.attachLongPressStart([]() {
    Piezo.warn();
    GooglyEyesApp::getInstance().shouldExitApp = true;
  });
}

void GooglyEyesApp::update() {
  Button.tick();  // Process button events
}

void GooglyEyesApp::render() {
  static unsigned long lastDrawTime = 0;
  unsigned long currentTime = millis();
  if (currentTime - lastDrawTime >= 200) {
    lastDrawTime = currentTime;

    // Erase previous pupils by overdrawing with white
    Display.fillCircle(prev_xl, prev_yl, pupilRadius, TFT_WHITE);
    Display.fillCircle(prev_xr, prev_yr, pupilRadius, TFT_WHITE);

    // Generate new pupil positions
    float angleL = random(0, 6283) / 1000.0; // 0 to ~2π
    float rL = sqrt(random(0, 10001) / 10000.0);
    int dxL = rL * rx_safe * cos(angleL);
    int dyL = rL * ry_safe * sin(angleL);
    int xl = w4 + dxL;
    int yl = h4 * 2 + dyL;

    float angleR = random(0, 6283) / 1000.0;
    float rR = sqrt(random(0, 10001) / 10000.0);
    int dxR = rR * rx_safe * cos(angleR);
    int dyR = rR * ry_safe * sin(angleR);
    int xr = w4 * 3 + dxR;
    int yr = h4 * 2 + dyR;

    // Draw new pupils
    Display.fillCircle(xl, yl, pupilRadius, TFT_BLACK);       // Left pupil
    Display.fillCircle(xr, yr, pupilRadius, TFT_BLACK);       // Right pupil

    // Store positions for next erase
    prev_xl = xl;
    prev_yl = yl;
    prev_xr = xr;
    prev_yr = yr;
  }
}

bool GooglyEyesApp::shouldExit() {
  return shouldExitApp;
}
