# SmallTV Boilerplate

This project is a boilerplate designed for easy development of applications for the SmallTV (most likely clone) product using the ESP8266 microcontroller. It provides a structured framework to manage multiple apps and hardware components, enabling rapid prototyping and development.

## Features
- **App Management**: Easily add, switch, and manage multiple apps.
- **Hardware Abstraction**: Simplified interfaces for display, touch button and piezo buzzer.
- **Splash Screen**: Customizable startup screen.
- **Extensibility**: Add new apps with minimal effort.

## Setup
1. Clone the repository to your local machine.
2. Install the required libraries for Arduino development.
3. Upload the code to your SmallTV using the Arduino IDE or PlatformIO.

## File Structure
- **src/**: Contains the main application code.
- **core/**: Core functionality, including app management and navigation.
- **hardware/**: Hardware abstraction for display and piezo buzzer.
- **apps/**: Placeholder for user-defined app implementations.
- **boards/**: Boards definition

## Documentation
https://kamilkrz.github.io/smalltv-boilerplate/index.html

## Adding New Apps
1. Create a new folder under `apps/` for your app.
2. Implement the app by inheriting from the base `App` class.
3. Register the app in `src/main.cpp` using `AppCollection::addApp()`.

## Usage
1. Power on the ESP8266.
2. The splash screen will display for a few seconds.
3. Use the main menu to navigate between apps.

## Roadmap
- [ ] Hardware:
  - [x] Handle screen
  - [x] Handle touch button
  - [x] Handle piezo buzzer
  - [ ] Handle different variants of smalltv
  - [ ] Handle LED (Present, hidden in enclosure but hey, still part of hardware specs)
    - [ ] It's used on versions with translucent case. 
  - [ ] Improve documentation for hardware abstraction layers.
  - [ ] Handle ESP32 version?
    - [ ] Confirm that some versions are bare without serial. 
- [x] Implement Apps
  - [x] Implement app abstraction layer
  - [x] Implement app dynamic collection
  - [x] Example app with WiFi capabilities
  - [X] "Core" apps for menu / settings funcionalities (almost working) 
- [ ] Easy build process 
  - [x] Move project from Arduino to PlatformIO
  - [x] Build through Github Actions
    - [x] Fix issue with variables in doxygen
    - [x] Pull request workflow - and its awesome
  - [ ] Handle Arduino compatibility
  - [ ] Serial Web flasher
  - [ ] OTA for hardare without serial
    - [ ] Investigate SmallTV OTA to be albe to painlessly put it on top of their firmware
  - [ ] Stop relay on hacked deps
    - [ ] Kindly ask about agrrement on eSPI_menu quirks from personal fork
    - [ ] Same for DigiFont
- [ ] Implement unit tests for core functionality.
  - [ ] Learn how to write tests
  - [ ] Learn how to implement tests in platformIO
- [ ] Documentation
  - [x] Automatic workflow to build from doxygen
  - [x] Create a (detailed) guide for adding new apps.
  - [ ] Add documentation about used libraries and explain why those
    - [ ] Add URLs to OneButton 
    - [ ] Add URLs to TFT_eSPI.
    - [ ] Add URLs to eSPI_menu
    - [ ] Add URLs to digifont
    - [ ] And many more
