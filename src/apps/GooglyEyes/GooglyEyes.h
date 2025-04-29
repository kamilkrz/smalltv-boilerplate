
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
};

#endif  // GOOGLYEYES_H