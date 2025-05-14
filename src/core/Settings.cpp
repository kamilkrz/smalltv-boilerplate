#include "Settings.h"

#include <ArduinoJson.h>
#include <LittleFS.h>

Settings settings;

void Settings::registerSetting(const String& key, const String& defaultValue, const String& description) {
  if (dynamicSettings.find(key) == dynamicSettings.end()) {
    dynamicSettings[key] = defaultValue;
  }
  settingDescriptions[key] = description;
}

String Settings::getSetting(const String& key, const String& fallback) const {
  auto it = dynamicSettings.find(key);
  return it != dynamicSettings.end() ? it->second : fallback;
}

void Settings::loadSettings() {
  Serial.println("[Settings] loadSettings() called");
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
  Serial.print("[Settings] settings.json size: ");
  Serial.println(size);
  if (size > 1024) {
    Serial.println("Settings file too large, using default settings.");
    file.close();
    return;
  }

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, file);
  file.close();

  if (error) {
    Serial.print("Failed to parse settings file, using default settings. Error: ");
    Serial.println(error.c_str());
    return;
  }

  // Load dynamic settings
  JsonObject dynamicSettingsObj = doc["dynamicSettings"].as<JsonObject>();
  Serial.println("[Settings] Loading dynamicSettings:");
  for (JsonObject::iterator it = dynamicSettingsObj.begin(); it != dynamicSettingsObj.end(); ++it) {
    Serial.print("  ");
    Serial.print(it->key().c_str());
    Serial.print(" = ");
    Serial.println(it->value().as<String>());
    settings.dynamicSettings[it->key().c_str()] = it->value().as<String>();
  }

  // Load setting descriptions
  JsonObject settingDescriptionsObj = doc["settingDescriptions"].as<JsonObject>();
  Serial.println("[Settings] Loading settingDescriptions:");
  for (JsonObject::iterator it = settingDescriptionsObj.begin(); it != settingDescriptionsObj.end(); ++it) {
    Serial.print("  ");
    Serial.print(it->key().c_str());
    Serial.print(" = ");
    Serial.println(it->value().as<String>());
    settings.settingDescriptions[it->key().c_str()] = it->value().as<String>();
  }

  Serial.println("Settings loaded successfully.");
}

void Settings::saveSettings() {
  Serial.println("[Settings] saveSettings() called");
  if (!LittleFS.begin()) {
    Serial.println("Failed to mount LittleFS, cannot save settings.");
    return;
  }

  File file = LittleFS.open("/settings.json", "w");
  if (!file) {
    Serial.println("Failed to open settings file for writing.");
    return;
  }

  JsonDocument doc;

  JsonObject dynamicSettingsObj = doc["dynamicSettings"].to<JsonObject>();
  Serial.println("[Settings] Saving dynamicSettings:");
  for (const auto& pair : settings.dynamicSettings) {
    Serial.print("  ");
    Serial.print(pair.first);
    Serial.print(" = ");
    Serial.println(pair.second);
    dynamicSettingsObj[pair.first] = pair.second;
  }

  JsonObject settingDescriptionsObj = doc["settingDescriptions"].to<JsonObject>();
  Serial.println("[Settings] Saving settingDescriptions:");
  for (const auto& pair : settings.settingDescriptions) {
    Serial.print("  ");
    Serial.print(pair.first);
    Serial.print(" = ");
    Serial.println(pair.second);
    settingDescriptionsObj[pair.first] = pair.second;
  }

  if (serializeJson(doc, file) == 0) {
    Serial.println("Failed to write settings to file.");
  } else {
    Serial.println("Settings saved successfully.");
  }

  file.close();
}
