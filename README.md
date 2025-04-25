# SmallTV ESP8266 Boilerplate

This project is a boilerplate designed for easy development of applications for the SmallTV (most likely clone) product using the ESP8266 microcontroller. It provides a structured framework to manage multiple apps and hardware components, enabling rapid prototyping and development.

## Features
- **App Management**: Easily add, switch, and manage multiple apps.
- **Hardware Abstraction**: Simplified interfaces for display, touch button and piezo buzzer.
- **Splash Screen**: Customizable startup screen.
- **Extensibility**: Add new apps with minimal effort.

## Setup
1. Clone the repository to your local machine.
2. Install the required libraries for Arduino development.
3. Upload the code to your ESP8266 using the Arduino IDE or PlatformIO.

## File Structure
- **src/**: Contains the main application code.
- **core/**: Core functionality, including app management and navigation.
- **hardware/**: Hardware abstraction for display and piezo buzzer.
- **apps/**: Placeholder for user-defined app implementations.
- **boards/**: Boards definition

## Adding New Apps
1. Create a new folder under `apps/` for your app.
2. Implement the app by inheriting from the base `App` class.
3. Register the app in `src/main.cpp` using `AppCollection::addApp()`.

## Usage
1. Power on the ESP8266.
2. The splash screen will display for a few seconds.
3. Use the main menu to navigate between apps.

## TODO
- [x] Implement Apps
  - [x] Implement app abstraction layer
  - [x] Implement app dynamic collection
  - [ ] Example app with WiFi capabilities 
- [ ] Hardware:
  - [x] Handle screen
  - [x] Handle touch button
  - [x] Handle piezo buzzer
  - [ ] Handle different variants of smalltv
  - [ ] Handle LED (Present, hidden in enclosure but hey, still part of hardware specs)
  - [ ] Improve documentation for hardware abstraction layers.
  - [ ] Handle ESP32 version?
- [x] Move project from Arduino to PlatformIO
  - [ ] Handle Arduino compatibility
  - [ ] Add documentation about used libraries and explain why those
- [ ] Implement unit tests for core functionality.
  - [ ] Learn how to write tests
  - [ ] Learn how to implement tests in platformIO
- [x] Create a detailed guide for adding new apps.
