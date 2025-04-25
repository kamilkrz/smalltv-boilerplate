#ifndef SETTINGS_APP_H
#define SETTINGS_APP_H

#include <WiFiManager.h>

#include "core/App.h"

class SettingsApp : public App {
 public:
  static SettingsApp& getInstance();
  void init() override;
  void update() override;
  void render() override;
  bool shouldExit() override;

 private:
  SettingsApp();
  ~SettingsApp();
  SettingsApp(const SettingsApp&) = delete;
  SettingsApp& operator=(const SettingsApp&) = delete;

  bool exitApp = false;
  WiFiManager wifiManager;
};

#endif  // SETTINGS_APP_H
