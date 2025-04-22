#include "ctTimer.h"

#include <functional>

ctTimer::ctTimer() : running(false), paused(false), duration(0), start_time(0), pause_offset(0), onDebug(nullptr) {}

void ctTimer::setEventCallback(std::function<void(const char*)> callback) {
  onDebug = callback;
}

void ctTimer::reset() {
  running = false;
  paused = false;
  // duration = 0;
  start_time = 0;
  pause_offset = 0;

  if (onDebug) onDebug("Timer reset.");
}

void ctTimer::start() {
  if (running) {
    if (onDebug) onDebug("Timer already running!");
    return;
  }
  running = true;
  paused = false;
  pause_offset = 0;
  start_time = millis();

  if (onDebug) onDebug("Timer started.");
}

void ctTimer::stop() {
  if (!running) {
    if (onDebug) onDebug("Timer already stopped!");
    return;
  }
  running = false;
  paused = false;

  if (onDebug) onDebug("Timer stopped.");
}

void ctTimer::pause() {
  if (running && !paused) {
    paused = true;
    pause_offset += millis() - start_time;

    if (onDebug) onDebug("Timer paused.");
  }
}

void ctTimer::resume() {
  if (running && paused) {
    paused = false;
    start_time = millis();

    if (onDebug) onDebug("Timer resumed.");
  }
}

void ctTimer::tick() {
  if (running && !paused) {
    unsigned long elapsed = millis() - start_time + pause_offset;
    if (onTickCallback) onTickCallback();
    if (elapsed >= duration) {
      stop();
      if (onFinishCallback) onFinishCallback();
      if (onDebug) onDebug("Timer ended and reset.");
    }
  }
}

bool ctTimer::isRunning() const {
  return running;
}

bool ctTimer::isPaused() const {
  return paused;
}

unsigned long ctTimer::getRemainingTime() const {
  if (!running) return duration;

  unsigned long elapsed = paused ? pause_offset : millis() - start_time + pause_offset;
  return (elapsed >= duration) ? 0 : duration - elapsed;
}

unsigned long ctTimer::getRemainingMinutes() const {
  return getRemainingTime() / 60000;
}

unsigned long ctTimer::getRemainingSeconds() const {
  return (getRemainingTime() % 60000) / 1000;
}

void ctTimer::setDuration(unsigned long minutes, unsigned long seconds) {
  if (!running) {
    duration = (minutes * 60 + seconds) * 1000;
  } else {
    if (onDebug) onDebug("Cannot set duration while timer is running!");
  }
}

void ctTimer::setTickCallback(std::function<void()> callback) {
  onTickCallback = callback;
}
void ctTimer::setFinishCallback(std::function<void()> callback) {
  onFinishCallback = callback;
}
