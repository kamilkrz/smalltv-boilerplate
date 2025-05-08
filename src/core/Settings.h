#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <map>
#include <string>

struct Settings {
  // Register a setting with a default value
  void registerSetting(const String& key, const String& defaultValue);

  // Retrieve a setting value, or return a fallback if not found
  String getSetting(const String& key, const String& fallback = "") const;

  // Dynamic settings storage
  std::map<String, String> dynamicSettings;

  void saveSettings();  // Save settings to LittleFS
  void loadSettings();  // Load settings from LittleFS
};

extern Settings settings;

#endif  // SETTINGS_H
