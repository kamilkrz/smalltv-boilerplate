#include "MainMenu.h"

#include "SharedState.h"
#include "hardware/Button.h"
#include "hardware/Display.h"
#include "hardware/Piezo.h"

MainMenu& MainMenu::getInstance() {
  static MainMenu instance;  // Guaranteed to be destroyed and instantiated on first use
  return instance;
}

MainMenu::MainMenu() {
  menu = new eSPIMenu::Menu(&Display);
}

MainMenu::~MainMenu() {
  delete menu;
}

void MainMenu::init() {
  menu->reset();
  prepMenuSpecs();
  menu->setTitle("Pomodoro Timer");
  menu->addItem("Basic Pomodoro", eSPIMenu::State::selected);
  menu->addItem("  55m+5m", eSPIMenu::State::none);
  menu->addItem("Open Clock", eSPIMenu::State::none);
  menu->addItem("Googly Eyes", eSPIMenu::State::none);
  menu->setStatus("Press: cycle, Hold: select");

  Button.attachClick(handleShortClick);          // Attach static function
  Button.attachLongPressStart(handleLongClick);  // Attach static function
  menu->show();
}

void MainMenu::update() {
  Button.tick();  // Process button events
}

void MainMenu::render() {
}

void MainMenu::handleShortClick() {
  MainMenu& instance = getInstance();
  int prevIndex = instance.menu->getSelectedItem();
  instance.menu->down();  // Navigate down on Button click
  int newIndex = instance.menu->getSelectedItem();
  Piezo.beep(PIEZO_PIN, 500, 50);
  if (prevIndex > newIndex) {
    delay(50);
    Piezo.beep(PIEZO_PIN, 300, 50);
  }
}

void MainMenu::handleLongClick() {
  MainMenu& instance = getInstance();
  Serial.println(instance.menu->getSelectedText());
  Piezo.beep(PIEZO_PIN, 300, 50);
  delay(50);
  Piezo.beep(PIEZO_PIN, 500, 50);
  // Example: Transition to googly eyes task on a specific menu item
  if (strcmp(instance.menu->getSelectedText(), "Googly Eyes") == 0) {
    SharedState::getInstance().setState(STATE_GOOGLYEYES);
  }
}

void MainMenu::prepMenuSpecs() {
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
  menu->getStatusSpec().setMargins(1, 1, 1, 1);  // Top, Left, Bottom, Right
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
}