#include "Settings.h"

#include <ArduinoJson.h>
#include <LittleFS.h>

Settings settings;

void Settings::registerSetting(const String& key, const String& defaultValue) {
  if (dynamicSettings.find(key) == dynamicSettings.end()) {
    dynamicSettings[key] = defaultValue;  // Add default if not present
  }
}

String Settings::getSetting(const String& key, const String& fallback) const {
  auto it = dynamicSettings.find(key);
  return it != dynamicSettings.end() ? it->second : fallback;
}

void Settings::loadSettings() {
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

  JsonDocument doc;  // Replaced DynamicJsonDocument with JsonDocument
  DeserializationError error = deserializeJson(doc, file);
  file.close();

  if (error) {
    Serial.println("Failed to parse settings file, using default settings.");
    return;
  }


  // Load dynamic settings
  JsonObject dynamicSettingsObj = doc["dynamicSettings"].as<JsonObject>();
  for (JsonObject::iterator it = dynamicSettingsObj.begin(); it != dynamicSettingsObj.end(); ++it) {
    settings.dynamicSettings[it->key().c_str()] = it->value().as<String>();
  }


  Serial.println("Settings loaded successfully.");
}

void Settings::saveSettings() {
  if (!LittleFS.begin()) {
    Serial.println("Failed to mount LittleFS, cannot save settings.");
    return;
  }

  File file = LittleFS.open("/settings.json", "w");
  if (!file) {
    Serial.println("Failed to open settings file for writing.");
    return;
  }

  JsonDocument doc;  // Replaced DynamicJsonDocument with JsonDocument

  JsonObject dynamicSettingsObj = doc["dynamicSettings"].to<JsonObject>();
  for (const auto& pair : settings.dynamicSettings) {
    dynamicSettingsObj[pair.first] = pair.second;
  }

  if (serializeJson(doc, file) == 0) {
    Serial.println("Failed to write settings to file.");
  } else {
    Serial.println("Settings saved successfully.");
  }

  file.close();
}
