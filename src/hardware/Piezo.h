#ifndef PIEZO_H
#define PIEZO_H

#include <Arduino.h>

/**
 * @brief Class to manage the piezo buzzer.
 */
class piezo {
 public:
  /**
   * @brief Constructor for the piezo class.
   * @param pin Pin connected to the piezo buzzer.
   */
  piezo(int pin);

  /**
   * @brief Generate a beep sound.
   * @param f Frequency of the beep.
   * @param d Duration of the beep in milliseconds.
   */
  void beep(int f, unsigned long d);

  /**
   * @brief Generate an acknowledgment sound.
   */
  void ack();

  /**
   * @brief Generate a warning sound.
   */
  void warn();

  /**
   * @brief Generate a normal notification sound.
   */
  void norm();

  /**
   * @brief Generate an error sound.
   */
  void err();

  /**
   * @brief Generate an alarm sound.
   */
  void alarm();
};

extern piezo Piezo;  // Declare Piezo as extern

#endif  // PIEZO_H