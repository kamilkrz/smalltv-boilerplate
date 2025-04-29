#ifndef BUZZER_H
#define BUZZER_H
#include <Arduino.h>

#ifndef PIEZO_PIN
#define PIEZO_PIN 4
#endif

/**
 * @class piezo
 * @brief A class to control a piezo buzzer.
 * @details This class provides methods to make the piezo buzzer beep at
 *          different frequencies and durations. It also includes methods
 *          for playing various sound effects such as acknowledgment,
 *          warning, normal, error, and alarm sounds.
 * @note The default pin for the piezo buzzer is defined as PIEZO_PIN (GPIO 4).
 * @ingroup Hardware
 */
class piezo {
 public:
  /**
   * @brief Constructor to initialize the piezo buzzer.
   * @param pin The pin connected to the piezo buzzer. Defaults to PIEZO_PIN.
   */
  piezo(int pin = PIEZO_PIN);

  /**
   * @brief Makes the piezo buzzer beep at a specific frequency and duration.
   * @param f Frequency of the beep in Hz.
   * @param d Duration of the beep in milliseconds. Defaults to 1000 ms.
   */
  void beep(int f, unsigned long d = 1000);

  /**
   * @brief Plays a normal notification sound.
   */
  void norm();

  /**
   * @brief Plays an acknowledgment sound.
   */
  void ack();

  /**
   * @brief Plays a warning sound.
   */
  void warn();

  /**
   * @brief Plays an error sound.
   */
  void err();

  /**
   * @brief Plays an alarm sound.
   */
  void alarm();
};

/**
 * @brief Global instance of the piezo class.
 */
extern piezo Piezo;

#endif  // BUZZER_H