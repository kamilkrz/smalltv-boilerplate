#include "SettingsApp.h"

#include "core/Settings.h"
#include "hardware/Display.h"

SettingsApp& SettingsApp::getInstance() {
  static SettingsApp instance;
  return instance;
}

SettingsApp::SettingsApp() : App("Settings") {}

SettingsApp::~SettingsApp() {}

void SettingsApp::init() {
  exitApp = false;

  // Configure WiFiManager
  WiFiManagerParameter brightnessParam("brightness", "Display Brightness", String(settings.displayBrightness).c_str(), 3);
  WiFiManagerParameter timezoneParam("timezone", "Timezone Offset", String(settings.timezoneOffset).c_str(), 4);
  WiFiManagerParameter formatParam("format", "24-Hour Format (1/0)", String(settings.use24HourFormat).c_str(), 2);

  wifiManager.addParameter(&brightnessParam);
  wifiManager.addParameter(&timezoneParam);
  wifiManager.addParameter(&formatParam);

  // Set callback to save parameters on change
  wifiManager.setSaveParamsCallback([&]() {
    settings.displayBrightness = atoi(brightnessParam.getValue());
    settings.timezoneOffset = atoi(timezoneParam.getValue());
    settings.use24HourFormat = atoi(formatParam.getValue());
    saveSettings();
    Serial.println("Settings saved from callback.");
  });

  // Start AP
  String apName = "ESP8266_Settings";
  wifiManager.setAPCallback([](WiFiManager* wm) {
    Display.fillScreen(TFT_BLACK);
    Display.setTextColor(TFT_WHITE);
    Display.setTextFont(2);
    Display.setCursor(0, 0);
    Display.println("AP Name: " + String(wm->getConfigPortalSSID()));
    Display.println("IP: " + WiFi.softAPIP().toString());
  });

  if (!wifiManager.startConfigPortal(apName.c_str())) {
    Serial.println("Conncection is closed");
  }

  exitApp = true;
}

void SettingsApp::update() {
  // No periodic updates needed
}

void SettingsApp::render() {
  // No rendering needed
}

bool SettingsApp::shouldExit() {
  return exitApp;
}
