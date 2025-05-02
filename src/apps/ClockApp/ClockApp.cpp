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
  ntpClient.setTimeOffset(settings.timezoneOffset * 3600);
  // Retrieve WiFi credentials from SettingsApp
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
  // Set time offset to UTC+1
}

ClockApp::~ClockApp() {
}

void ClockApp::displayTime(const String& time) {
  static unsigned long lastUpdateTime = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdateTime < 1000) return;
  lastUpdateTime = currentMillis;

  //   Display.fillScreen(TFT_BLACK);

  int w = Display.width();
  int h = Display.height();
  uint8_t th = 10;
  uint8_t wd = (w - th - 10) / 5;
  uint8_t yd = h / 2;
  digi.setSize1(wd - 3, yd, th);
  int y = digi.getHeight() / 2;

  digi.setColors(TFT_WHITE, TFT_BLACK);
  digi.drawDigit1(time[0], 0 * wd, y);
  digi.drawDigit1(time[1], 1 * wd, y);
  digi.setColors(((currentMillis / 1000) % 2) ? TFT_WHITE : TFT_BLACK, TFT_BLACK);
  digi.drawDigit1(time[2], (2 * wd) + (digi.getWidth() / 2), y);
  digi.setColors(TFT_WHITE, TFT_BLACK);
  digi.drawDigit1(time[3], 3 * wd, y);
  digi.drawDigit1(time[4], 4 * wd, y);
  //   for (size_t i = 0; i < time.length(); ++i) {
  //   }
}