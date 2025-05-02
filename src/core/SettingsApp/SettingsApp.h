#ifndef SETTINGS_APP_H
#define SETTINGS_APP_H

#include <WiFiManager.h>

#include "core/App.h"

/**
 * @class SettingsApp
 * @brief Application for managing settings, including WiFi configuration.
 *
 * This class is a singleton that provides functionality for initializing,
 * updating, rendering, and exiting the settings application.
 * @ingroup Apps
 */
class SettingsApp : public App {
 public:
  /**
   * @brief Get the singleton instance of the SettingsApp.
   * @return Reference to the singleton instance.
   */
  static SettingsApp& getInstance();

  void init() override;        // Inherited from App
  void update() override;      // Inherited from App
  void render() override;      // Inherited from App
  bool shouldExit() override;  // Inherited from App

 private:
  /**
   * @brief Private constructor for the singleton pattern.
   */
  SettingsApp();

  /**
   * @brief Destructor for the SettingsApp.
   */
  ~SettingsApp();

  SettingsApp(const SettingsApp&) = delete;             // Inherited behavior
  SettingsApp& operator=(const SettingsApp&) = delete;  // Inherited behavior

  bool exitApp = false;     // Flag indicating whether the application should exit
  WiFiManager wifiManager;  // WiFiManager instance for managing WiFi settings
};

#endif  // SETTINGS_APP_H
