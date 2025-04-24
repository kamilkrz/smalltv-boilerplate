#include "AppLauncher.h"

#include "core/AppCollection.h"
#include "hardware/Button.h"
#include "hardware/Display.h"
#include "hardware/Piezo.h"

AppLauncher& AppLauncher::getInstance() {
  static AppLauncher instance;  // Guaranteed to be destroyed and instantiated on first use
  return instance;
}

AppLauncher::AppLauncher() : App("Main Menu") {
  menu = new eSPIMenu::Menu(&Display);
}

AppLauncher::~AppLauncher() {
  delete menu;
}

void AppLauncher::init() {
  Display.fillScreen(TFT_DARKGREY);
  menu->reset();
  prepMenuSpecs();
  menu->setTitle("Main Menu");

  // Dynamically add apps from AppCollection to the menu
  AppCollection& appCollection = AppCollection::getInstance();
  std::vector<String> apps = appCollection.listApps();
  if (apps.empty()) {
    menu->addItem("No Apps Available", eSPIMenu::State::disabled);
    menu->setStatus("No apps found");
  }
  for (size_t i = 1; i < apps.size(); ++i) {
    Serial.println(apps[i].c_str());
    menu->addItem(apps[i].c_str(), i == 1 ? eSPIMenu::State::selected : eSPIMenu::State::none);
  }

  menu->setStatus("Press: cycle, Hold: select");

  Button.attachClick(handleShortClick);          // Attach static function
  Button.attachLongPressStart(handleLongClick);  // Attach static function
  menu->show();
}

void AppLauncher::update() {
  Button.tick();  // Process button events
}

void AppLauncher::render() {
}

void AppLauncher::handleShortClick() {
  AppLauncher& instance = getInstance();
  int prevIndex = instance.menu->getSelectedItem();
  instance.menu->down();  // Navigate down on Button click
  int newIndex = instance.menu->getSelectedItem();
  Piezo.beep(500, 50);
  if (prevIndex > newIndex) {
    delay(50);
    Piezo.beep(300, 50);
  }
}

void AppLauncher::prepMenuSpecs() {
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

void AppLauncher::handleLongClick() {
  AppLauncher& instance = getInstance();
  unsigned int selectedIndex = instance.menu->getSelectedItem() + 1;  // +1 to skip the Launcher
  AppCollection& appCollection = AppCollection::getInstance();

  if (selectedIndex >= 0 && selectedIndex < appCollection.listApps().size()) {
    Piezo.beep(300, 50);
    delay(50);
    Piezo.beep(500, 50);
    appCollection.switchToApp(selectedIndex);
  }
}