#ifndef CT_TIMER_H
#define CT_TIMER_H

#include <Arduino.h>

#include <functional>

class ctTimer {
 private:
  bool running;
  bool paused;
  unsigned long duration;
  unsigned long start_time;
  unsigned long pause_offset;
  std::function<void(const char*)> onDebug;
  std::function<void()> onFinishCallback;
  std::function<void()> onTickCallback;

 public:
  ctTimer();
  void setEventCallback(std::function<void(const char*)> callback);
  void setTickCallback(std::function<void()> callback);
  void setFinishCallback(std::function<void()> callback);
  void start();
  void stop();
  void pause();
  void resume();
  void tick();
  bool isRunning() const;
  bool isPaused() const;
  unsigned long getRemainingTime() const;
  unsigned long getRemainingMinutes() const;
  unsigned long getRemainingSeconds() const;
  void setDuration(unsigned long minutes, unsigned long seconds);
  void reset();
};

#endif  // CT_TIMER_H