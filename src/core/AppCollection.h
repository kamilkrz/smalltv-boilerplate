#ifndef APP_COLLECTION_H
#define APP_COLLECTION_H

#include <memory>
#include <string>
#include <vector>

#include "App.h"

class AppCollection {
 public:
  static AppCollection& getInstance() {
    static AppCollection instance;
    return instance;
  }

  void addApp(App* app) {
    Serial.print("Adding app: ");
    Serial.println(app->appName);
    apps.push_back(app);
  }

  void switchToApp(unsigned int index) {
    if (index >= 0 && index < apps.size()) {
      currentAppIndex = index;
      apps[currentAppIndex]->init();
    }
  }

  void updateCurrentApp() {
    if (!apps.empty()) {
      apps[currentAppIndex]->update();
    }
  }

  void renderCurrentApp() {
    if (!apps.empty()) {
      apps[currentAppIndex]->render();
    }
  }

  bool shouldExitCurrentApp() {
    if (!apps.empty()) {
      return apps[currentAppIndex]->shouldExit();
    }
    return false;
  }

  void returnToMainMenu() {
    switchToApp(0);  // Assuming the first app is the main menu
  }

  std::vector<String> listApps() const {
    std::vector<String> appNames;
    for (const App* app : apps) {
      if (app && app->appName) {
        Serial.print("Pushing: ");
        Serial.println(app->appName);
        appNames.emplace_back(app->appName);  // Use std::string to manage memory safely
      }
    }
    return appNames;
  }

 private:
  AppCollection() = default;
  ~AppCollection() = default;

  AppCollection(const AppCollection&) = delete;
  AppCollection& operator=(const AppCollection&) = delete;

  std::vector<App*> apps;
  int currentAppIndex = 0;
};

#endif  // APP_COLLECTION_H
