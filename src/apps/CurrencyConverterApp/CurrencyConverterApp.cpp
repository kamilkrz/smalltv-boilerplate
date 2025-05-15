#include "CurrencyConverterApp.h"

#include "hardware/Hardware.h"
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>  // Add this for HTTPS support
#include <ArduinoJson.h>
#include "core/Settings.h"
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

CurrencyConverterApp& CurrencyConverterApp::getInstance() {
  static CurrencyConverterApp instance;
  return instance;
}

CurrencyConverterApp::CurrencyConverterApp() : App("Currency Converter") {
  settings.registerSetting("currency_app_id", "", "OpenExchangeRates APP_ID");
  settings.registerSetting("currency_from", "VES", "Currency FROM (e.g. VES)");
  settings.registerSetting("currency_to", "PLN", "Currency TO (e.g. PLN)");
}

CurrencyConverterApp::~CurrencyConverterApp() {}

void CurrencyConverterApp::init() {
  shouldExitApp = false;
  forceUpdate = true;
  Display.drawSplashScreen("Currency Converter", "Initializing...");
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
  Button.attachClick([]() {
    Piezo.err();
  });
  Button.attachLongPressStart([]() {
    Piezo.ack();
    CurrencyConverterApp::getInstance().shouldExitApp = true;
  });
  fetchExchangeRate();
}

void CurrencyConverterApp::update() {
  fetchExchangeRate();
  Button.tick();
}

void CurrencyConverterApp::render() {
  
  unsigned long currentMillis = millis();
  String fromCurrency = settings.getSetting("currency_from", "VES");
  String toCurrency = settings.getSetting("currency_to", "PLN");

  if (exchangeRate > 0) {
    // Redraw only if an hour has passed or the rate has changed
    if ((currentMillis - lastRenderMillis >= 3600000UL) || forceUpdate) {
      Display.drawSplashScreen(String(exchangeRate,7), fromCurrency + " to " + toCurrency);
      lastRenderMillis = currentMillis;
      forceUpdate = false;
    }
    // else: do not redraw
  } else {
    // If rate is not known, allow normal rendering (e.g., during fetch)
    Display.drawSplashScreen("Currency Converter", "Fetching rate...");
    lastRenderMillis = currentMillis; // reset timer so next known rate will show immediately
  }
}

bool CurrencyConverterApp::shouldExit() {
  return shouldExitApp;
}

void CurrencyConverterApp::fetchExchangeRate() {
  String appId = settings.getSetting("currency_app_id", "");
  String fromCurrency = settings.getSetting("currency_from", "VES");
  String toCurrency = settings.getSetting("currency_to", "PLN");

  // Try to use cached JSON if less than 1 hour old
  const char* cacheFile = "/currency_cache.json";
  String payload;
  bool usedCache = false;

  // Get current epoch time via NTP (like in ClockApp)
  WiFiUDP ntpUDP;
  NTPClient ntpClient(ntpUDP, "pool.ntp.org");
  ntpClient.begin();
  ntpClient.update();
  unsigned long currentEpoch = ntpClient.getEpochTime();

  if (LittleFS.begin()) {
    if (LittleFS.exists(cacheFile)) {
      File f = LittleFS.open(cacheFile, "r");
      if (f) {
        payload = f.readString();
        f.close();
        // Parse JSON to get timestamp
        JsonDocument tempDoc;
        DeserializationError tempError = deserializeJson(tempDoc, payload);
        if (!tempError && tempDoc["timestamp"].is<unsigned long>()) {
          unsigned long cachedTimestamp = tempDoc["timestamp"].as<unsigned long>();
          if (currentEpoch > 0 && cachedTimestamp > 0 && (currentEpoch - cachedTimestamp < 3600)) {
            Serial.println("  Using cached JSON from LittleFS.");
            usedCache = true;
          }
        }
      }
    }
    LittleFS.end();
  }

  if (!usedCache) {
    Serial.println("  Fetching fresh JSON from OpenExchangeRates API.");
    if (appId.length() == 0) {
      Serial.println("  No APP_ID set, aborting fetch.");
      exchangeRate = -1;
      lastApiCall = millis();
      Display.drawSplashScreen("No APP_ID set", "Please configure in settings.");
      delay(3000);
      shouldExitApp = true;
      return;
    }

    HTTPClient http;
    WiFiClientSecure wifiClient;  // Use WiFiClientSecure for HTTPS
    wifiClient.setInsecure();     // Skip certificate validation
    String url = "https://openexchangerates.org/api/latest.json?app_id=" + appId;
    Serial.print("  Requesting URL: ");
    Serial.println(url);

    http.begin(wifiClient, url);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
      payload = http.getString();

      // Store bare JSON to LittleFS
      if (LittleFS.begin()) {
        File f = LittleFS.open(cacheFile, "w");
        if (f) {
          f.print(payload);
          f.close();
        }
        LittleFS.end();
      }
    } else {
      Serial.print("  HTTP error: ");
      Serial.println(httpCode);
      http.end();
      lastApiCall = millis();
      return;
    }
    http.end();
  }

  // Parse JSON (from cache or fresh)
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (!error) {
    // Also update cache validation logic when we successfully parse JSON
    float fromToUsd = doc["rates"][fromCurrency];
    float toToUsd = doc["rates"][toCurrency];
    if (fromToUsd > 0 && toToUsd > 0) {
      exchangeRate = toToUsd / fromToUsd;
    } else {
      Serial.println("  Invalid rates received.");
    }
  } else {
    Serial.print("  JSON parse error: ");
    Serial.println(error.c_str());
  }

  lastApiCall = millis();
}
