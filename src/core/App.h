#ifndef APP_H
#define APP_H

/**
 * @file App.h
 * @brief Abstract base class for applications.
 * @defgroup Apps Apps functionality
 * @{
 */

/**
 * @class App
 * @brief Abstract base class for applications.
 *
 * This class provides a framework for creating applications with
 * initialization, update, rendering, and exit logic. It enforces
 * singleton behavior by deleting the copy constructor and assignment operator.
 */
class App {
 protected:
  /**
   * @brief Protected constructor to initialize the application name.
   * @param name The name of the application.
   */
  App(const String& name) : appName(name) {}

 public:
  /**
   * @brief Virtual destructor.
   */
  virtual ~App() = default;

  /**
   * @brief Initialize the application.
   *
   * This method is called to perform any setup required before the application runs.
   */
  virtual void init() = 0;

  /**
   * @brief Update the application logic.
   *
   * This method is called periodically to update the state of the application.
   */
  virtual void update() = 0;

  /**
   * @brief Render the application's output.
   *
   * This method is called to render the application's output, such as drawing to a display.
   */
  virtual void render() = 0;

  /**
   * @brief Check if the application should exit.
   * @return True if the application should exit, false otherwise.
   */
  virtual bool shouldExit() = 0;

  /**
   * @brief Deleted copy constructor to enforce singleton behavior.
   */
  App(const App&) = delete;

  /**
   * @brief Deleted assignment operator to enforce singleton behavior.
   */
  App& operator=(const App&) = delete;

  /**
   * @brief The name of the application.
   */
  const String appName;
};

#endif     // APP_H
/** @} */  // End of APP group