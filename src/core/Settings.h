#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <map>
#include <string>

/**
 * @brief A structure to manage application settings.
 * 
 * This structure provides methods to register, retrieve, save, and load
 * dynamic settings for the application.
 * @ingroup Apps
 */
struct Settings {
  /**
   * @brief Register a setting with a default value.
   * 
   * @param key The key identifying the setting.
   * @param defaultValue The default value for the setting.
   */
  void registerSetting(const String& key, const String& defaultValue);

  /**
   * @brief Retrieve a setting value, or return a fallback if not found.
   * 
   * @param key The key identifying the setting.
   * @param fallback The fallback value to return if the setting is not found.
   * @return The value of the setting or the fallback value.
   */
  String getSetting(const String& key, const String& fallback = "") const;

  /**
   * @brief Dynamic settings storage.
   * 
   * A map that holds all registered settings and their values.
   */
  std::map<String, String> dynamicSettings;

  /**
   * @brief Save settings to LittleFS.
   * 
   * This method persists the current settings to the filesystem.
   */
  void saveSettings();

  /**
   * @brief Load settings from LittleFS.
   * 
   * This method loads settings from the filesystem into memory.
   */
  void loadSettings();
};

/**
 * @brief Global instance of the Settings structure.
 */
extern Settings settings;

#endif  // SETTINGS_H
