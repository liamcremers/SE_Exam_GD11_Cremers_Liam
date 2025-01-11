#pragma once

#include <string>

class SetupContext {
public:
    SetupContext(const std::string& title, int width, int height)
        : windowTitle(title), windowWidth(width), windowHeight(height) {}

    void setWindowSize(int width, int height) {
        windowWidth = width;
        windowHeight = height;
    }

    void setWindowTitle(const std::string& title) {
        windowTitle = title;
    }

    const std::string& getWindowTitle() const { return windowTitle; }
    int getWindowWidth() const { return windowWidth; }
    int getWindowHeight() const { return windowHeight; }

private:
    std::string windowTitle;
    int windowWidth;
    int windowHeight;

    friend class ToyPainter;
};