#ifndef CURRENCY_CONVERTER_APP_H
#define CURRENCY_CONVERTER_APP_H

#include <Arduino.h>
#include "core/App.h"

class CurrencyConverterApp : public App {
 public:
  static CurrencyConverterApp& getInstance();
  void init() override;
  void update() override;
  void render() override;
  bool shouldExit() override;

 private:
  CurrencyConverterApp();
  ~CurrencyConverterApp();
  CurrencyConverterApp(const CurrencyConverterApp&) = delete;
  CurrencyConverterApp& operator=(const CurrencyConverterApp&) = delete;

  float exchangeRate = 0.0;
  unsigned long lastApiCall = 0;
  const unsigned long apiCallInterval = 3600000;  // 1 hour in milliseconds
  void fetchExchangeRate();
  bool shouldExitApp = false;
};

#endif  // CURRENCY_CONVERTER_APP_H
