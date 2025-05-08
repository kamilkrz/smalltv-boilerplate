#include <Arduino.h>
// #include <DigiFont.h>

#include <memory>
#include <vector>

// CORE
#include "core/AppCollection.h"
#include "core/AppLauncher/AppLauncher.h"
#include "core/Settings.h"
#include "core/SettingsApp/SettingsApp.h"
#include "hardware/Hardware.h"
// APPS
#include "apps/ClockApp/ClockApp.h"
#include "apps/GooglyEyes/GooglyEyes.h"
#include "apps/PomodoroApp/PomodoroApp.h"
#include "apps/MatrixApp/MatrixApp.h"

void setup() {
  // SERIAL
  Serial.begin(115200);
  delay(1000);

  // Initialize the Display display
  Display.init();
  Display.fillScreen(TFT_BLACK);
  Display.setBacklight(255);

  // Show splash screen
  Display.drawSplashScreen("SmallTV Boilerplate", "by kamilkrz");
  delay(2000);  // Display splash screen for 2 seconds

  // Prep Piezo
  Piezo.alarm();
  delay(200);
  // Register apps
  settings.loadSettings();  // Load settings from LittleFS
  AppCollection& appCollection = AppCollection::getInstance();
  // Main Menu Should be added first!
  appCollection.addApp(&AppLauncher::getInstance());
  // Then add other app
  appCollection.addApp(&ClockApp::getInstance());
  appCollection.addApp(&PomodoroApp::getInstance());
  appCollection.addApp(&GooglyEyesApp::getInstance());
  appCollection.addApp(&MatrixApp::getInstance());

  // Add settings app at the end
  appCollection.addApp(&SettingsApp::getInstance());
  settings.saveSettings();  // Save settings to LittleFS

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
