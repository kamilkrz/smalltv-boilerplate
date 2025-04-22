#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <Arduino.h>
#include <eSPI_Menu.h>

#include "App.h"


class MainMenu : public App {
 public:
  static MainMenu& getInstance();

  void init() override;
  void update() override;
  void render() override;

 private:
  MainMenu();
  ~MainMenu();

  MainMenu(const MainMenu&) = delete;
  MainMenu& operator=(const MainMenu&) = delete;

  eSPIMenu::Menu* menu;

  static void handleShortClick();
  static void handleLongClick();
  void prepMenuSpecs();
};

#endif  // MAIN_MENU_H