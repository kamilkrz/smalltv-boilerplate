#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>

struct Settings {
  uint8_t displayBrightness = 255;        // Default brightness (0-255)
  uint8_t timezoneOffset = 0;             // Default timezone offset in hours
  bool use24HourFormat = true;            // Default time format (24-hour)

  // Add more settings as needed
};

extern Settings settings;

void loadSettings();  // Load settings from LittleFS
void saveSettings();  // Save settings to LittleFS

#endif  // SETTINGS_H
