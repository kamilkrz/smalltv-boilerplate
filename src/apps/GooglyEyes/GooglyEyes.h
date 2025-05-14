#ifndef GOOGLYEYES_H
#define GOOGLYEYES_H

#include <Arduino.h>

#include "core/App.h"

/**
 * @class GooglyEyesApp
 * @brief Basic application to display googly eyes on the screen.
 * @ingroup Apps
 */
class GooglyEyesApp : public App {
 public:
  static GooglyEyesApp& getInstance();
  void init() override;
  void update() override;
  void render() override;
  bool shouldExit() override;

 private:
  GooglyEyesApp();
  ~GooglyEyesApp();
  GooglyEyesApp(const GooglyEyesApp&) = delete;
  GooglyEyesApp& operator=(const GooglyEyesApp&) = delete;
  bool shouldExitApp = false;
  int w4, h4, rx, ry, pupilRadius, rx_safe, ry_safe;
  int prev_xl, prev_yl, prev_xr, prev_yr;
};

#endif  // GOOGLYEYES_H