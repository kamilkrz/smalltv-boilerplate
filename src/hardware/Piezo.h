#ifndef BUZZER_H
#define BUZZER_H
#include <Arduino.h>

#ifndef PIEZO_PIN
#define PIEZO_PIN 4
#endif

class piezo {
 public:
  piezo(int pin = PIEZO_PIN) {
    pinMode(PIEZO_PIN, OUTPUT);
  }
  void beep(uint8_t p, int f, unsigned long d = 1000) {
    tone(p, f, d);
  }
};

extern piezo Piezo;

#endif  // BUZZER_H