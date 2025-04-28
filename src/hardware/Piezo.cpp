#include "Piezo.h"

piezo::piezo(int pin) {
  pinMode(pin, OUTPUT);
}

void piezo::beep(int f, unsigned long d) {
  tone(PIEZO_PIN, f, d);
}

void piezo::ack() {
  beep(300, 50);
  delay(50);
  beep(500, 50);
}

void piezo::warn() {
  beep(500, 50);
  delay(50);
  beep(300, 50);
}

void piezo::norm() {
  beep(500, 50);
}

void piezo::err() {
  beep(500, 100);
  delay(120);
  beep(500, 100);
}

void piezo::alarm() {
  beep(500, 50);
  delay(50);
  beep(1000, 50);
  delay(50);
  beep(500, 50);
  delay(50);
  beep(1000, 50);
}

piezo Piezo(PIEZO_PIN);  // Define Piezo
