
#ifndef CLOCK_APP_H
#define CLOCK_APP_H

#include <Arduino.h>
#include <DigiFont.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "core/App.h"

/**
 * @class ClockApp
 * @brief Simple clock application.
 * @details This class provides functionality for displaying the current time
 *          using NTP (Network Time Protocol). It initializes the WiFi connection,
 *          retrieves the time from an NTP server, and displays it on the screen.
 * @ingroup Apps
 */
class ClockApp : public App {
 public:
  static ClockApp& getInstance();
  void init() override;
  void update() override;
  void render() override;
  bool shouldExit() override;  // No exit condition for this app

 private:
  ClockApp();
  ~ClockApp();
  ClockApp(const ClockApp&) = delete;
  ClockApp& operator=(const ClockApp&) = delete;

  DigiFont digi;
  WiFiUDP ntpUDP;
  NTPClient ntpClient{ntpUDP, "pool.ntp.org", 0, 60000};
  void displayTime(const String& time);
  bool shouldExitApp = false;
};

#endif
