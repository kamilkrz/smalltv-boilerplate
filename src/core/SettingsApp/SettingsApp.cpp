#include "SettingsApp.h"

#include "core/Settings.h"
#include "hardware/Hardware.h"

// Add this at the top of the file or in the class header (not shown here):
std::vector<String> parameterLabels;

SettingsApp& SettingsApp::getInstance() {
  static SettingsApp instance;
  return instance;
}

SettingsApp::SettingsApp() : App("Settings") {
}

SettingsApp::~SettingsApp() {
}

void SettingsApp::init() {
  Serial.println("[SettingsApp] init() called");
  exitApp = false;
  Display.drawSplashScreen("Settings", "connecting...");
  wifiManager.setConfigPortalBlocking(true);

  // Dynamically create WiFiManager parameters based on registered settings
  if (parameters.empty()) {
    Serial.println("[SettingsApp] Creating WiFiManager parameters:");
    parameterLabels.clear(); // Ensure no stale pointers
    for (const auto& pair : settings.dynamicSettings) {
      String label = pair.first;
      auto descIt = settings.settingDescriptions.find(pair.first);
      if (descIt != settings.settingDescriptions.end()) {
        label = descIt->second;
      }
      parameterLabels.push_back(label); // Store label to keep memory alive
      Serial.print("  Adding parameter: ID=");
      Serial.print(pair.first);
      Serial.print(", Label=");
      Serial.print(label.c_str());
      Serial.print(", Default=");
      Serial.println(pair.second);
      auto param = std::make_unique<WiFiManagerParameter>(
          pair.first.c_str(), parameterLabels.back().c_str(), pair.second.c_str(), 32);
      wifiManager.addParameter(param.get());
      parameters.push_back(std::move(param));  // Store unique_ptr in the vector
    }
  }

  // Set callback to save parameters on change
  wifiManager.setSaveParamsCallback([&]() {
    Serial.println("[SettingsApp] SaveParamsCallback triggered");
    for (const auto& param : parameters) {
      Serial.print("  Saving parameter: ID=");
      Serial.print(param->getID());
      Serial.print(", Value=");
      Serial.println(param->getValue());
      settings.dynamicSettings[param->getID()] = param->getValue();
    }
    settings.saveSettings();
    wifiManager.stopConfigPortal();
    exitApp = true;
  });
  wifiManager.setWebServerCallback([]() {
    Display.fillScreen(TFT_BLACK);
    Display.setTextColor(TFT_WHITE);
    Display.setTextFont(4);
    Display.setCursor(0, 0);
    Display.println("Settings");
    Display.setTextFont(2);
      Display.println("WiFi Name: " + WiFi.SSID());
      Display.println("IP Address: " + WiFi.localIP().toString());
    Display.println("Settings:");
    for (const auto& pair : settings.dynamicSettings) {
      Display.println(pair.first + ": " + pair.second);
    }
  });

  wifiManager.setAPCallback([](WiFiManager* wm) {
    Display.fillScreen(TFT_BLACK);
    Display.setTextColor(TFT_WHITE);
    Display.setTextFont(4);
    Display.setCursor(0, 0);
    Display.println("Settings");
    Display.setTextFont(2);
    Display.println("AP Name: " + String(wm->getConfigPortalSSID()));
    Display.println("IP: " + WiFi.softAPIP().toString());
    Display.println("Settings:");
    for (const auto& pair : settings.dynamicSettings) {
      Display.println(pair.first + ": " + pair.second);
    }
  });


  if (wifiManager.autoConnect("ESP8266_Settings")) {
    wifiManager.startConfigPortal();
  }
  
}

void SettingsApp::update() {
}

void SettingsApp::render() {
}

bool SettingsApp::shouldExit() {
  return exitApp;
}
