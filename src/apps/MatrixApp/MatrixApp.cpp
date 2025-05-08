#include <Arduino.h>
#include "MatrixApp.h"
#include "hardware/Hardware.h"

MatrixApp& MatrixApp::getInstance() {
    static MatrixApp instance;
    return instance;
}

MatrixApp::MatrixApp() : App("Matrix"), currentColor(TFT_GREEN) {}

MatrixApp::~MatrixApp() {}

void MatrixApp::init() {
    shouldExitApp = false;
    Display.fillScreen(TFT_BLACK);
    Display.setTextFont(2); // Set a Unicode-compatible font

    // Seed the random number generator to avoid repeating patterns
    randomSeed(analogRead(A0));

    for (int i = 0; i < MAX_COLUMNS; ++i) {
        columns[i] = random(Display.height());
    }

    Button.attachClick([]() {
        Piezo.ack();
        MatrixApp& instance = MatrixApp::getInstance();
        instance.changeColor();
    });

    Button.attachLongPressStart([]() {
        Piezo.warn();
        MatrixApp::getInstance().shouldExitApp = true;
    });
}

void MatrixApp::update() {
    Button.tick(); // Process button events
}

void MatrixApp::render() {
    static unsigned long lastUpdate = 0;
    unsigned long currentTime = millis();

    // Only update if 50ms have passed since the last update
    if (currentTime - lastUpdate < 50) {
        return;
    }
    lastUpdate = currentTime;

    int fH = Display.fontHeight();
    for (int i = 0; i < MAX_COLUMNS; ++i) {
        int x = i * FONT_WIDTH;
        int y = columns[i];

        // Draw the trailing characters with fading effect
        for (int j = 1; j <= Display.height() / fH + 5; ++j) { // Extend trail length to cover the screen
            int trailY = y - (j * fH);
            if (trailY >= 0 && trailY < Display.height()) {
                uint8_t fadeFactor = max(0, 255 - (j * 50)); // Ensure fadeFactor doesn't go negative

                // Extract RGB components from currentColor
                uint8_t r = (currentColor >> 11) & 0x1F;
                uint8_t g = (currentColor >> 5) & 0x3F;
                uint8_t b = currentColor & 0x1F;

                // Apply fade factor to each component
                r = (r * fadeFactor) / 255;
                g = (g * fadeFactor) / 255;
                b = (b * fadeFactor) / 255;

                // Reconstruct the faded color
                uint16_t trailColor = Display.color565(r << 3, g << 2, b << 3);

                Display.fillRect(x, trailY, FONT_WIDTH, fH, TFT_BLACK);
                Display.setTextColor(trailColor, TFT_BLACK);
                Display.drawChar(random(0x20, 0x7F), x, trailY); // ASCII range
            }
        }

        // Draw the leading character
        if (y < Display.height()) { // Ensure the leading character is within bounds
            char c = random(0x20, 0x7F); // ASCII range
            Display.setTextColor(TFT_WHITE, TFT_BLACK);
            Display.drawChar(c, x, y);
        }

        // Move the column down
        columns[i] += fH;

        // Reset column if it goes off-screen
        if (columns[i] > Display.height() + (5 * fH)) {
            columns[i] = 0 - (random(0, 15) * fH);
        }
    }
}

void MatrixApp::changeColor() {
    currentColor = random(0xFFFF); // Randomize text color
}

bool MatrixApp::shouldExit() {
    return shouldExitApp; // Prevent auto-exit
}
