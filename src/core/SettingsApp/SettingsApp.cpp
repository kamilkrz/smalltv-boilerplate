#include "SettingsApp.h"

#include "core/Settings.h"
#include "hardware/Hardware.h"

SettingsApp& SettingsApp::getInstance() {
  static SettingsApp instance;
  return instance;
}

SettingsApp::SettingsApp() : App("Settings") {
}

SettingsApp::~SettingsApp() {
}

void SettingsApp::init() {
  exitApp = false;
  Display.drawSplashScreen("Settings", "connecting...");
  wifiManager.setConfigPortalBlocking(true);

  // Dynamically create WiFiManager parameters based on registered settings
  if (parameters.empty()) {
    for (const auto& pair : settings.dynamicSettings) {
      auto param = std::make_unique<WiFiManagerParameter>(
          pair.first.c_str(), pair.first.c_str(), pair.second.c_str(), 32);
      wifiManager.addParameter(param.get());
      parameters.push_back(std::move(param));  // Store unique_ptr in the vector
    }
  }

  // Set callback to save parameters on change
  wifiManager.setSaveParamsCallback([&]() {
    for (const auto& param : parameters) {
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
