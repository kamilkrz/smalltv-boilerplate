#include "CurrencyConverterApp.h"

#include "hardware/Hardware.h"
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>  // Add this for HTTPS support
#include <ArduinoJson.h>
#include "core/Settings.h"
#include <ESP8266WiFi.h>
#include <LittleFS.h>

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
  static unsigned long lastRenderMillis = 0;
  static float lastRenderedRate = -1;
  unsigned long currentMillis = millis();
  String fromCurrency = settings.getSetting("currency_from", "VES");
  String toCurrency = settings.getSetting("currency_to", "PLN");

  if (exchangeRate > 0) {
    // Redraw only if an hour has passed or the rate has changed
    if ((currentMillis - lastRenderMillis >= 3600000UL) || (exchangeRate != lastRenderedRate)) {
      Display.drawSplashScreen(String(exchangeRate,7), fromCurrency + " to " + toCurrency + ":");
      lastRenderMillis = currentMillis;
      lastRenderedRate = exchangeRate;
    }
    // else: do not redraw
  } else {
    // If rate is not known, allow normal rendering (e.g., during fetch)
    Display.drawSplashScreen("Currency Converter", "Fetching rate...");
    lastRenderMillis = currentMillis; // reset timer so next known rate will show immediately
    lastRenderedRate = -1;
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
  if (LittleFS.begin()) {
    if (LittleFS.exists(cacheFile)) {
      File f = LittleFS.open(cacheFile, "r");
      if (f) {
        String line = f.readStringUntil('\n');
        unsigned long cachedMillis = line.toInt();
        payload = f.readString(); // read the rest as JSON
        f.close();
        unsigned long now = millis();
        if (now >= cachedMillis && (now - cachedMillis < 3600000UL)) {
          Serial.println("  Using cached JSON from LittleFS.");
          usedCache = true;
        }
      }
    }
    LittleFS.end();
  }

  if (!usedCache) {
    if (appId.length() == 0) {
      Serial.println("  No APP_ID set, aborting fetch.");
      exchangeRate = -1;
      lastApiCall = millis();
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

      // Store to LittleFS with timestamp and JSON
      if (LittleFS.begin()) {
        File f = LittleFS.open(cacheFile, "w");
        if (f) {
          f.println(String(millis()));
          f.print(payload); // no extra newline
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
    float fromToUsd = doc["rates"][fromCurrency];
    float toToUsd = doc["rates"][toCurrency];
    Serial.print("  ");
    Serial.print(fromCurrency);
    Serial.print(" to USD: ");
    Serial.println(fromToUsd, 6);
    Serial.print("  ");
    Serial.print(toCurrency);
    Serial.print(" to USD: ");
    Serial.println(toToUsd, 6);
    if (fromToUsd > 0 && toToUsd > 0) {
      exchangeRate = toToUsd / fromToUsd;
      Serial.print("  Calculated ");
      Serial.print(fromCurrency);
      Serial.print(" to ");
      Serial.print(toCurrency);
      Serial.print(" rate: ");
      Serial.println(exchangeRate, 6);
    } else {
      Serial.println("  Invalid rates received.");
    }
  } else {
    Serial.print("  JSON parse error: ");
    Serial.println(error.c_str());
  }

  lastApiCall = millis();
}
