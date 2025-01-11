#pragma once

#include <SDL.h>
#include <string>

#include "Color.h"

class SetupContext;

class ToyPainter {
public:
    ToyPainter(const SetupContext& setupCtx);
    ~ToyPainter();

    void PollEvents();
    void Present() const;
    bool IsRunning() const;

    void resizeWindow(int width, int height) const;
    void setTitle(const std::string& title) const;

    void Clear(const Color& color) const;
    void FillCircle(int x, int y, int radius, const Color& color) const;
    void FillCircle(int x, int y, int radius, const Color& color, int segments) const;
    void FillRect(int x, int y, int width, int height, const Color& color) const;

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
};
