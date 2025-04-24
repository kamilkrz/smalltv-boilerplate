#ifndef APP_H
#define APP_H

class App {
 protected:
  // Protected constructor to initialize appName
  App(const String& name) : appName(name) {}

 public:
  virtual ~App() = default;

  // Method to initialize the app
  virtual void init() = 0;

  // Method to update the app logic
  virtual void update() = 0;

  // Method to render the app's output
  virtual void render() = 0;

  virtual bool shouldExit() = 0;

  // Deleted copy constructor and assignment operator to enforce singleton behavior
  App(const App&) = delete;
  App& operator=(const App&) = delete;

  // Field to store the app name
  const String appName;
};

#endif  // APP_H