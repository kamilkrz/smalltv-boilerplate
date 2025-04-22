#ifndef APP_H
#define APP_H

class App {
 protected:
  // Protected constructor to prevent direct instantiation
  App() = default;

 public:
  virtual ~App() = default;

  // Method to initialize the app
  virtual void init() = 0;

  // Method to update the app logic
  virtual void update() = 0;

  // Method to render the app's output
  virtual void render() = 0;

  // Deleted copy constructor and assignment operator to enforce singleton behavior
  App(const App&) = delete;
  App& operator=(const App&) = delete;
};

#endif  // APP_H