#include <Arduino.h>
// #include <DigiFont.h>

#include "apps/GooglyEyes.h"
#include "apps/MainMenu.h"
#include "apps/SharedState.h"
#include "hardware/Display.h"
#include "hardware/Piezo.h"

void setup() {
  // SERIAL
  Serial.begin(115200);
  delay(1000);

  // Initialize the Display display
  Display.init();
  Display.fillScreen(RGBto565(0, 60, 60));
  Display.setBacklight(255);
  // Prep Piezo
  Piezo.beep(PIEZO_PIN, 600, 200);
  delay(200);

  SharedState::getInstance().setState(STATE_MENU);

  // Configure Button actions
}

AppState previousState = STATE_INIT;  // Track the previous state

void loop() {
  AppState state = SharedState::getInstance().getState();

  // Call init only when the state changes
  if (state != previousState) {
    switch (state) {
      case STATE_MENU:
        MainMenu::getInstance().init();
        break;
      case STATE_GOOGLYEYES:
        googlyEyes::getInstance().init();
        break;
      // Add other cases as needed
      default:
        break;
    }
    previousState = state;  // Update the previous state
  }

  // Handle updates and rendering
  switch (state) {
    case STATE_MENU:
      MainMenu::getInstance().update();
      MainMenu::getInstance().render();
      break;
    case STATE_GOOGLYEYES:
      googlyEyes::getInstance().update();
      googlyEyes::getInstance().render();
      break;
    // Add other cases as needed
    default:
      break;
  }
}
