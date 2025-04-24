#ifndef APP_LAUNCHER_H
#define APP_LAUNCHER_H

#include <Arduino.h>
#include <eSPI_Menu.h>

#include "core/App.h"

class AppLauncher : public App {
 public:
  static AppLauncher& getInstance();

  void init() override;
  void update() override;
  void render() override;
  bool shouldExit() override {
    return false;  // No exit condition for this app
  }

 private:
  AppLauncher();
  ~AppLauncher();

  AppLauncher(const AppLauncher&) = delete;
  AppLauncher& operator=(const AppLauncher&) = delete;

  eSPIMenu::Menu* menu;

  static void handleShortClick();
  static void handleLongClick();
  void prepMenuSpecs();
};

#endif  // APP_LAUNCHER_H