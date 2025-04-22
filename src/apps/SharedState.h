#ifndef SHAREDSTATE_H
#define SHAREDSTATE_H

enum AppState { STATE_INIT,
                STATE_MENU,
                STATE_GOOGLYEYES,
                STATE_OTHER };  // Define app states

class SharedState {
 private:
  AppState currentState;
  SharedState() : currentState(STATE_MENU) {}  // Default state is menu

 public:
  static SharedState& getInstance() {
    static SharedState instance;
    return instance;
  }

  AppState getState() const { return currentState; }
  void setState(AppState state) { currentState = state; }

  // Delete copy constructor and assignment operator
  SharedState(const SharedState&) = delete;
  SharedState& operator=(const SharedState&) = delete;
};

#endif  // SHAREDSTATE_H
