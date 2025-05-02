#ifndef APP_LAUNCHER_H
#define APP_LAUNCHER_H

#include <Arduino.h>
#include <eSPI_Menu.h>

#include "core/App.h"

/**
 * @class AppLauncher
 * @brief Application for launching and managing other applications.
 *
 * This class provides functionality for initializing, updating, rendering,
 * and managing the application launcher interface.
 * @ingroup Apps
 */
class AppLauncher : public App {
 public:
  /**
   * @brief Get the singleton instance of the AppLauncher.
   * @return Reference to the singleton instance.
   */
  static AppLauncher& getInstance();

  void init() override;         // Inherited from App
  void update() override;       // Inherited from App
  void render() override;       // Inherited from App
  bool shouldExit() override {  // Inherited from App
    return false;               // No exit condition for this app
  }

 private:
  /**
   * @brief Private constructor for the singleton pattern.
   */
  AppLauncher();

  /**
   * @brief Destructor for the AppLauncher.
   */
  ~AppLauncher();

  AppLauncher(const AppLauncher&) = delete;             // Inherited behavior
  AppLauncher& operator=(const AppLauncher&) = delete;  // Inherited behavior

  eSPIMenu::Menu* menu;  // Pointer to the menu instance

  /**
   * @brief Handle a short button click event.
   */
  static void handleShortClick();

  /**
   * @brief Handle a long button click event.
   */
  static void handleLongClick();

  /**
   * @brief Prepare the specifications for the menu.
   */
  void prepMenuSpecs();
};

#endif  // APP_LAUNCHER_H