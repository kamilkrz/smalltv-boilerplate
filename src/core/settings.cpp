#include "settings.h"

#include <ArduinoJson.h>
#include <LittleFS.h>

Settings settings;

void loadSettings() {
  if (!LittleFS.begin()) {
    Serial.println("Failed to mount LittleFS, using default settings.");
    return;
  }

  File file = LittleFS.open("/settings.json", "r");
  if (!file) {
    Serial.println("Settings file not found, using default settings.");
    return;
  }

  size_t size = file.size();
  if (size > 1024) {
    Serial.println("Settings file too large, using default settings.");
    file.close();
    return;
  }

  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, file);
  file.close();

  if (error) {
    Serial.println("Failed to parse settings file, using default settings.");
    return;
  }

  settings.displayBrightness = doc["displayBrightness"] | 255;
  settings.timezoneOffset = doc["timezoneOffset"] | 0;
  settings.use24HourFormat = doc["use24HourFormat"] | true;

  Serial.println("Settings loaded successfully.");
}

void saveSettings() {
  if (!LittleFS.begin()) {
    Serial.println("Failed to mount LittleFS, cannot save settings.");
    return;
  }

  File file = LittleFS.open("/settings.json", "w");
  if (!file) {
    Serial.println("Failed to open settings file for writing.");
    return;
  }

  StaticJsonDocument<512> doc;
  doc["displayBrightness"] = settings.displayBrightness;
  doc["timezoneOffset"] = settings.timezoneOffset;
  doc["use24HourFormat"] = settings.use24HourFormat;

  if (serializeJson(doc, file) == 0) {
    Serial.println("Failed to write settings to file.");
  } else {
    Serial.println("Settings saved successfully.");
  }

  file.close();
}
