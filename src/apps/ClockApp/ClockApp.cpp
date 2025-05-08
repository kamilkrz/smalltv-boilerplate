#include "ClockApp.h"

#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "core/Settings.h"
#include "hardware/Hardware.h"

WiFiUDP ntpUDP;
NTPClient ntpClient(ntpUDP, "pool.ntp.org");

ClockApp& ClockApp::getInstance() {
  static ClockApp instance;
  return instance;
}

void ClockApp::init() {
  shouldExitApp = false;
  timezoneOffset = settings.getSetting("ClockApp_timezoneOffset").toInt();  // Retrieve timezoneOffset from settings
  ntpClient.setTimeOffset(timezoneOffset * 3600);  // Use local timezoneOffset
  Display.fillScreen(TFT_BLACK);
  Display.setTextColor(TFT_WHITE);
  Display.setTextFont(4);
  Display.setTextDatum(MC_DATUM);
  Display.drawString("Connecting to WiFi...", Display.width() / 2, Display.height() / 2);
  Display.setTextDatum(TL_DATUM);

  // Attempt to connect using saved credentials
  WiFi.begin();
  unsigned long startAttemptTime = millis();
  const unsigned long timeout = 30000;  // 30 seconds timeout

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
    delay(500);
  }

  if (WiFi.status() != WL_CONNECTED) {
    // Connection failed, display timeout message and exit app
    Display.fillScreen(TFT_BLACK);
    Display.drawString("WiFi Timeout!", Display.width() / 2, Display.height() / 2);
    delay(3000);  // Show timeout message for 3 seconds
    shouldExitApp = true;
    return;
  }

  // Connection successful
  Display.fillScreen(TFT_BLACK);
  ntpClient.begin();
  ntpClient.update();

  Button.attachClick([]() {
    Piezo.err();
  });
  Button.attachLongPressStart([]() {
    Piezo.ack();
    ClockApp::getInstance().shouldExitApp = true;
  });
}

void ClockApp::update() {
  ntpClient.update();
  Button.tick();
}

void ClockApp::render() {
  displayTime(ntpClient.getFormattedTime());
}

bool ClockApp::shouldExit() {
  return shouldExitApp;
}

ClockApp::ClockApp() : App("Clock"),
                       digi(
                           [](int x0, int x1, int y, int c) { Display.drawFastHLine(x0, y, x1 - x0 + 1, c); },
                           [](int x, int y0, int y1, int c) { Display.drawFastVLine(x, y0, y1 - y0 + 1, c); },
                           [](int x, int y, int w, int h, int c) { Display.fillRect(x, y, w, h, c); }) {
  settings.registerSetting("ClockApp_timezoneOffset", "0");   // Default to UTC
  settings.registerSetting("ClockApp_use24HourFormat", "1");  // Default to 24-hour format
}

ClockApp::~ClockApp() {
}

void ClockApp::displayTime(const String& time) {
  static unsigned long lastUpdateTime = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdateTime < 1000) return;
  lastUpdateTime = currentMillis;

  String formattedTime = time;

  // Check if 24-hour format is disabled
  if (settings.getSetting("ClockApp.use24HourFormat").toInt() == 0) {
    int hours = time.substring(0, 2).toInt();
    bool isPM = hours >= 12;
    if (hours == 0) {
      hours = 12;  // Midnight
    } else if (hours > 12) {
      hours -= 12;  // Convert to 12-hour format
    }
    formattedTime = (hours < 10 ? "0" : "") + String(hours) + time.substring(2);
    formattedTime += isPM ? " PM" : " AM";
  }

  int w = Display.width();
  int h = Display.height();
  uint8_t th = 10;
  uint8_t wd = (w - th - 10) / 5;
  uint8_t yd = h / 2;
  digi.setSize1(wd - 3, yd, th);
  int y = digi.getHeight() / 2;

  digi.setColors(TFT_WHITE, TFT_BLACK);
  digi.drawDigit1(formattedTime[0], 0 * wd, y);
  digi.drawDigit1(formattedTime[1], 1 * wd, y);
  digi.setColors(((currentMillis / 1000) % 2) ? TFT_WHITE : TFT_BLACK, TFT_BLACK);
  digi.drawDigit1(formattedTime[2], (2 * wd) + (digi.getWidth() / 2), y);
  digi.setColors(TFT_WHITE, TFT_BLACK);
  digi.drawDigit1(formattedTime[3], 3 * wd, y);
  digi.drawDigit1(formattedTime[4], 4 * wd, y);

  // Display AM/PM if in 12-hour format
  if (settings.getSetting("ClockApp.use24HourFormat").toInt() == 0) {
    Display.setTextColor(TFT_WHITE, TFT_BLACK);
    Display.setTextFont(2);
    Display.setCursor(w - 40, h - 20);
    Display.print(formattedTime.substring(6));  // Print "AM" or "PM"
  }
}