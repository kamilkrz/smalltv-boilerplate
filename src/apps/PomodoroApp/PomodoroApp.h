#ifndef POMODORO_APP_H
#define POMODORO_APP_H

#include <Arduino.h>
#include <DigiFont.h>
#include <eSPI_Menu.h>

#include "core/App.h"
#include "ctTimer.h"

enum class State { Timer,
                   Menu };  // Define states

/**
 * @class PomodoroApp
 * @brief Simple pomodoro timer application.
 * @apps_group
 */
class PomodoroApp : public App {
 public:
  static PomodoroApp& getInstance();
  void init() override;
  void update() override;
  void render() override;
  bool shouldExit() override;  // No exit condition for this app

 private:
  PomodoroApp();
  ~PomodoroApp();
  PomodoroApp(const PomodoroApp&) = delete;
  PomodoroApp& operator=(const PomodoroApp&) = delete;

  static void handleTimerClick();
  static void handleTimerLongClick();
  static void handleMenuShortClick();
  static void handleMenuLongClick();

  State currentState;  // Track current state
  ctTimer timer;
  DigiFont digi;
  eSPIMenu::Menu* menu;
  bool shouldExitApp = false;

  void toggleDuration();
  void displayDigit(int digit, int x, int y, int fg_col, int bg_col);
  void displayTime(bool forceRefresh = false);

  void initTimerState();  // Helper for Timer state initialization
  void initMenuState();   // Helper for Menu state initialization
};

#endif  // POMODORO_APP_H
