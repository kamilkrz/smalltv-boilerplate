#include <Arduino.h>
// #include <DigiFont.h>

#include <memory>
#include <vector>

// CORE
#include "core/AppCollection.h"
#include "core/AppLauncher/AppLauncher.h"
#include "hardware/Display.h"
#include "hardware/Piezo.h"
// APPS
#include "apps/GooglyEyes/GooglyEyes.h"
#include "apps/PomodoroApp/PomodoroApp.h"

// Add MainMenu, GooglyEyes, and PomodoroApp to the collection
void setup() {
  // SERIAL
  Serial.begin(115200);
  delay(1000);

  // Initialize the Display display
  Display.init();
  Display.fillScreen(RGBto565(0, 60, 60));
  Display.setBacklight(255);
  // Prep Piezo
  Piezo.beep(600, 200);
  delay(200);

  // Register apps
  AppCollection& appCollection = AppCollection::getInstance();
  appCollection.addApp(&AppLauncher::getInstance());  // Main Menu
  appCollection.addApp(&googlyEyes::getInstance());
  appCollection.addApp(&PomodoroApp::getInstance());

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
