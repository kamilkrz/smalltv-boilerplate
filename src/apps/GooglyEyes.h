#ifndef GOOGLYEYES_H
#define GOOGLYEYES_H

#include <Arduino.h>

#include "App.h"

class googlyEyes : public App {
 public:
  static googlyEyes& getInstance();
  void init() override;
  void update() override;
  void render() override;

 private:
  googlyEyes();
  ~googlyEyes();
  googlyEyes(const googlyEyes&) = delete;
  googlyEyes& operator=(const googlyEyes&) = delete;
};

#endif  // GOOGLYEYES_H