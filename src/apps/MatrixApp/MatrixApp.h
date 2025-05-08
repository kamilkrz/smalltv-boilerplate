#ifndef MATRIX_APP_H
#define MATRIX_APP_H

#include "core/App.h"
#include "hardware/Hardware.h"

#define MAX_COLUMNS 20
#define FONT_WIDTH 12
#define FONT_HEIGHT 16

class MatrixApp : public App {
public:
    static MatrixApp& getInstance();
    void init() override;
    void update() override;
    void render() override;
    bool shouldExit() override;
    void changeColor();

private:
    MatrixApp();
    ~MatrixApp();
    MatrixApp(const MatrixApp&) = delete;
    MatrixApp& operator=(const MatrixApp&) = delete;

    int columns[MAX_COLUMNS];
    uint16_t currentColor = TFT_GREEN;
    bool shouldExitApp = false;
};

#endif // MATRIX_APP_H
