#include "ToyPainter.h"

#include <vector>
#include <cmath>

#include "SetupContext.h"

ToyPainter::ToyPainter(const SetupContext& setupCtx)
    : running(true) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(setupCtx.windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, setupCtx.windowWidth, setupCtx.windowHeight, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

ToyPainter::~ToyPainter() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void ToyPainter::PollEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
    }
}

void ToyPainter::Present() const
{
    SDL_RenderPresent(renderer);
}

bool ToyPainter::IsRunning() const {
    return running;
}

void ToyPainter::resizeWindow(int width, int height) const
{
    SDL_SetWindowSize(window, width, height);
}

void ToyPainter::setTitle(const std::string& title) const
{
    SDL_SetWindowTitle(window, title.c_str());
}

void ToyPainter::Clear(const Color& color) const
{
    const auto& sdlColor = color.ToSdlColor();
    SDL_SetRenderDrawColor(renderer, sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
    SDL_RenderClear(renderer);
}

void ToyPainter::FillCircle(int x, int y, int radius, const Color& color) const
{
    FillCircle(x, y, radius, color, 100);
}

void ToyPainter::FillCircle(int centerX, int centerY, int radius, const Color& color, int segments) const
{
    std::vector<SDL_Vertex> vertices;
    std::vector<int> indices;

    const auto& sdlColor = color.ToSdlColor();

    vertices.push_back({
        SDL_FPoint{static_cast<float>(centerX), static_cast<float>(centerY)},
        sdlColor,
        SDL_FPoint{0.5f, 0.5f}
        });

    for (int i = 0; i <= segments; ++i) {
	    const float theta = 2.0f * M_PI * i / segments;
	    const float x = centerX + radius * std::cos(theta);
	    const float y = centerY + radius * std::sin(theta);

        vertices.push_back({
            SDL_FPoint{static_cast<float>(x), static_cast<float>(y)},
            sdlColor,
            SDL_FPoint{0.5f + 0.5f * std::cos(theta), 0.5f + 0.5f * std::sin(theta)}
            });
    }

    for (int i = 1; i <= segments; ++i) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    SDL_RenderGeometry(renderer, nullptr, vertices.data(), vertices.size(), indices.data(), indices.size());

}

void ToyPainter::FillRect(int x, int y, int width, int height, const Color& color) const
{
    // Set the drawing color
    const auto& sdlColor = color.ToSdlColor();
    SDL_SetRenderDrawColor(renderer, sdlColor.r, sdlColor.g, sdlColor.g, sdlColor.a);

    // Define the rectangle
    const SDL_Rect rect{x, y, width, height};

    // Render the rectangle
    SDL_RenderFillRect(renderer, &rect);
}
