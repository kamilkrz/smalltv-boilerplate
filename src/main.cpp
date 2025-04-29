#include <Arduino.h>
// #include <DigiFont.h>

#include <memory>
#include <vector>

// CORE
#include "core/AppCollection.h"
#include "core/AppLauncher/AppLauncher.h"
#include "core/Settings.h"
#include "core/SettingsApp/SettingsApp.h"
#include "hardware/Display.h"
#include "hardware/Piezo.h"
// APPS
#include "apps/ClockApp/ClockApp.h"
#include "apps/GooglyEyes/GooglyEyes.h"
#include "apps/PomodoroApp/PomodoroApp.h"

void setup() {
  // SERIAL
  Serial.begin(115200);
  delay(1000);

  // Initialize the Display display
  Display.init();
  Display.fillScreen(TFT_DARKGREY);
  Display.setBacklight(255);

  // Show splash screen
  Display.setTextColor(TFT_WHITE);
  Display.setTextFont(4);
  Display.setTextDatum(MC_DATUM);
  Display.drawString("ESP8266 Clock", Display.width() / 2, Display.height() / 2);
  // back to default
  Display.setTextDatum(TL_DATUM);
  ;

  delay(2000);  // Display splash screen for 2 seconds

  // Prep Piezo
  Piezo.alarm();
  delay(200);
  loadSettings();  // Load settings from LittleFS
  saveSettings();  // Save settings to LittleFS
  // Register apps
  AppCollection& appCollection = AppCollection::getInstance();
  // Main Menu Should be added first!
  appCollection.addApp(&AppLauncher::getInstance());
  // Then add other app
  appCollection.addApp(&ClockApp::getInstance());
  appCollection.addApp(&PomodoroApp::getInstance());
  appCollection.addApp(&GooglyEyesApp::getInstance());
  // So this should be last app? No need to do  this but looks like a good idea
  appCollection.addApp(&SettingsApp::getInstance());

  // Start with the first app
  appCollection.switchToApp(0);
}

void loop() {
  AppCollection& appCollection = AppCollection::getInstance();

  appCollection.updateCurrentApp();
  appCollection.renderCurrentApp();

  // Check if the current app wants to exit
  if (appCollection.shouldExitCurrentApp()) {
    appCollection.returnToMainMenu();
  }
}
