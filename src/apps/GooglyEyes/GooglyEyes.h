#ifndef GOOGLYEYES_H
#define GOOGLYEYES_H

#include <Arduino.h>

#include "core/App.h"

class googlyEyes : public App {
 public:
  static googlyEyes& getInstance();
  void init() override;
  void update() override;
  void render() override;
  bool shouldExit() override;

 private:
  googlyEyes();
  ~googlyEyes();
  googlyEyes(const googlyEyes&) = delete;
  googlyEyes& operator=(const googlyEyes&) = delete;
  bool shouldExitApp = false;
};

#endif  // GOOGLYEYES_H