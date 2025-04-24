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

void setup() {
  // SERIAL
  Serial.begin(115200);
  delay(1000);

  // Initialize the Display display
  Display.init();
  Display.fillScreen(RGBto565(0, 60, 60));
  Display.setBacklight(255);

  // Show splash screen
  Display.setTextColor(RGBto565(255, 255, 255));
  Display.setTextFont(2);
  Display.setTextSize(2);
  Display.setTextDatum(MC_DATUM);
  Display.drawString("ESP8266 Clock", Display.width() / 2, Display.height() / 2);
  // back to default
  Display.setTextDatum(TL_DATUM);
  Display.setTextSize(1);

  delay(2000);  // Display splash screen for 2 seconds

  // Prep Piezo
  Piezo.beep(600, 200);
  delay(200);

  // Register apps
  AppCollection& appCollection = AppCollection::getInstance();
  // Main Menu Should be added first!
  appCollection.addApp(&AppLauncher::getInstance());
  // Then add other apps
  appCollection.addApp(&PomodoroApp::getInstance());
  appCollection.addApp(&googlyEyes::getInstance());

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
