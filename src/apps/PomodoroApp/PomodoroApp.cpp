#include "PomodoroApp.h"

#include <Arduino.h>
#include <DigiFont.h>

#include "ctTimer.h"
#include "hardware/Button.h"
#include "hardware/Display.h"
#include "hardware/Piezo.h"

PomodoroApp& PomodoroApp::getInstance() {
  static PomodoroApp instance;
  return instance;
}

PomodoroApp::PomodoroApp() : currentState(State::Menu),
                             App("Pomodoro Timer"),
                             digi(
                                 [](int x0, int x1, int y, int c) { Display.drawFastHLine(x0, y, x1 - x0 + 1, c); },
                                 [](int x, int y0, int y1, int c) { Display.drawFastVLine(x, y0, y1 - y0 + 1, c); },
                                 [](int x, int y, int w, int h, int c) { Display.fillRect(x, y, w, h, c); }) {
  menu = new eSPIMenu::Menu(&Display);
}

PomodoroApp::~PomodoroApp() {
  delete menu;
}

unsigned long PomodoroApp::toggleDuration() {
  static const unsigned long durations[] = {1, 5, 10, 25, 45};
  unsigned long minutes = timer.getRemainingMinutes();

  for (size_t i = 0; i < sizeof(durations) / sizeof(durations[0]); ++i) {
    if (minutes < durations[i]) {
      timer.setDuration(durations[i], 0);
      return durations[i];
    }
  }
  timer.setDuration(durations[0], 0);  // Reset to the first duration
  return durations[0];
}

void PomodoroApp::displayDigit(int digit, int x, int y, int fg_col, int bg_col) {
  digi.setColors(fg_col, bg_col);
  digi.drawDigit1(digit, x, y);
}

void PomodoroApp::displayTime(bool forceRefresh) {
  static unsigned long previousSeconds = 0;
  static unsigned long previousMinutes = 0;
  static unsigned long lastUpdateTime = 0;

  unsigned long currentMillis = millis();
  if (!forceRefresh && (currentMillis - lastUpdateTime < 1000)) return;
  lastUpdateTime = currentMillis;

  unsigned long minutes = timer.getRemainingMinutes();
  unsigned long seconds = timer.getRemainingSeconds();

  int t = 10, w = (240 - t - 10) / 4, y = (240 - w * 2) / 2;
  digi.setSize1(w - 3, w * 2, t);

  int fg_col = (!timer.isRunning() && (currentMillis / 1000) % 2) ? RGBto565(0, 40, 0) : RGBto565(0, 220, 0);
  int bg_col = RGBto565(0, 40, 0);

  if (forceRefresh || minutes / 10 != previousMinutes / 10 || !timer.isRunning()) {
    displayDigit(minutes / 10, 0 * w, y, fg_col, bg_col);
  }
  if (forceRefresh || minutes % 10 != previousMinutes % 10 || !timer.isRunning()) {
    displayDigit(minutes % 10, 1 * w, y, fg_col, bg_col);
  }
  digi.setColors((seconds % 2) ? fg_col : bg_col, (seconds % 2) ? bg_col : fg_col);
  digi.drawDigit1(':', 2 * w + 5 - 3, y);

  if (forceRefresh || seconds / 10 != previousSeconds / 10 || !timer.isRunning()) {
    displayDigit(seconds / 10, t + 10 - 3 + 2 * w, y, fg_col, bg_col);
  }
  if (forceRefresh || seconds % 10 != previousSeconds % 10 || !timer.isRunning()) {
    displayDigit(seconds % 10, t + 10 - 3 + 3 * w, y, fg_col, bg_col);
  }

  previousMinutes = minutes;
  previousSeconds = seconds;
}

void PomodoroApp::init() {
  shouldExitApp = false;
  switch (currentState) {
    case State::Timer:
      initTimerState();
      break;
    case State::Menu:
      initMenuState();
      break;
  }
}

void PomodoroApp::update() {
  switch (currentState) {
    case State::Timer:
      timer.tick();
      Button.tick();
      break;
    case State::Menu:
      Button.tick();
      break;
  }
}

void PomodoroApp::render() {
  switch (currentState) {
    case State::Timer:
      displayTime();
      break;
    case State::Menu:
      break;
  }
}

bool PomodoroApp::shouldExit() {
  return shouldExitApp;
}

void PomodoroApp::initTimerState() {
  timer.reset();
  Display.fillScreen(TFT_BLACK);
  displayTime(true);
  timer.start();
  Button.attachClick(handleTimerClick);
  Button.attachLongPressStart(handleTimerLongClick);
}

void PomodoroApp::initMenuState() {
  Display.fillScreen(TFT_DARKGREY);
  menu->reset();
  menu->setInfiniteScroll(true);
  menu->getTitleSpec().setFont(4);
  menu->getTitleSpec().setMargins(10, 20, 10, 20);  // Top, Left, Bottom, Right
  menu->getTitleSpec().setBorder(1, 1, 1, 1);       // Top, Left, Bottom, Right
  menu->getTitleSpec().setItemColors(
      RGBto565(0, 0, 120),      // Background
      RGBto565(255, 255, 255),  // Foreground
      RGBto565(0, 60, 0),       // Selected Background
      RGBto565(255, 255, 255),  // Selected Foreground
      RGBto565(50, 50, 50),     // Disabled Background
      RGBto565(128, 128, 128)   // Disabled Foreground
  );
  menu->getTitleSpec().setBorderColors(
      RGBto565(0, 0, 0),    // Border
      RGBto565(0, 120, 0),  // Selected Border
      RGBto565(50, 50, 50)  // Disabled Border
  );

  // Configure item spec
  menu->getItemSpec().setFont(2);
  menu->getItemSpec().setMargins(2, 4, 2, 4);  // Top, Left, Bottom, Right
  menu->getItemSpec().setBorder(1, 1, 1, 1);   // Top, Left, Bottom, Right
  menu->getItemSpec().setItemColors(
      RGBto565(0, 0, 0),        // Background
      RGBto565(255, 255, 255),  // Foreground
      RGBto565(0, 60, 0),       // Selected Background
      RGBto565(255, 255, 255),  // Selected Foreground
      RGBto565(50, 50, 50),     // Disabled Background
      RGBto565(128, 128, 128)   // Disabled Foreground
  );
  menu->getItemSpec().setBorderColors(
      RGBto565(0, 0, 0),    // Border
      RGBto565(0, 120, 0),  // Selected Border
      RGBto565(50, 50, 50)  // Disabled Border
  );

  // Configure status spec
  menu->getStatusSpec().setFont(1);
  menu->getStatusSpec().setMargins(1, 1, 5, 1);  // Top, Left, Bottom, Right
  menu->getStatusSpec().setBorder(0, 0, 0, 0);   // Top, Left, Bottom, Right
  menu->getStatusSpec().setItemColors(
      RGBto565(50, 50, 50),     // Background
      RGBto565(128, 128, 128),  // Foreground
      RGBto565(0, 60, 0),       // Selected Background
      RGBto565(255, 255, 255),  // Selected Foreground
      RGBto565(50, 50, 50),     // Disabled Background
      RGBto565(128, 128, 128)   // Disabled Foreground
  );
  menu->getStatusSpec().setBorderColors(
      RGBto565(0, 0, 0),    // Border
      RGBto565(0, 120, 0),  // Selected Border
      RGBto565(50, 50, 50)  // Disabled Border
  );
  menu->setTitle("Options");
  menu->addItem("Start Timer", eSPIMenu::State::selected);
  menu->addItem("Set Duration", eSPIMenu::State::none);
  menu->addItem("Back to Main", eSPIMenu::State::none);
  menu->setStatus("Press: cycle, Hold: select");

  Button.attachClick(handleMenuShortClick);
  Button.attachLongPressStart(handleMenuLongClick);
  menu->show();
}

void PomodoroApp::handleTimerClick() {
  PomodoroApp& instance = getInstance();
  Piezo.beep(900, 50);

  if (!instance.timer.isRunning()) {
    instance.timer.start();
    instance.displayTime(true);
  } else if (instance.timer.isPaused()) {
    instance.timer.resume();
  } else {
    instance.timer.pause();
  }
}

void PomodoroApp::handleTimerLongClick() {
  PomodoroApp& instance = getInstance();
  Piezo.beep(900, 300);
  instance.currentState = State::Menu;
  instance.init();
}

void PomodoroApp::handleMenuShortClick() {
  PomodoroApp& instance = getInstance();
  int prevIndex = instance.menu->getSelectedItem();
  instance.menu->down();
  int newIndex = instance.menu->getSelectedItem();
  Piezo.beep(500, 50);
  if (prevIndex > newIndex) {
    delay(50);
    Piezo.beep(300, 50);
  }
}

void PomodoroApp::handleMenuLongClick() {
  PomodoroApp& instance = getInstance();
  Serial.println(instance.menu->getSelectedText());
  Piezo.beep(300, 50);
  delay(50);
  Piezo.beep(500, 50);

  if (strcmp(instance.menu->getSelectedText(), "Start Timer") == 0) {
    instance.currentState = State::Timer;
    instance.init();
  } else if (strcmp(instance.menu->getSelectedText(), "Set Duration") == 0) {
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "Duration: %lu min", instance.toggleDuration());
    instance.menu->setStatus(buffer);
    instance.menu->show();
  } else if (strcmp(instance.menu->getSelectedText(), "Back to Main") == 0) {
    instance.shouldExitApp = true;
  }
}