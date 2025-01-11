#pragma once

#include <SDL.h>
#include <random>

struct Color {
    Color(float r = 1.f, float g = 1.f, float b = 1.f) : r(r), g(g), b(b) {}

    float r;
    float g;
    float b;

    SDL_Color ToSdlColor() const
    {
        return SDL_Color{
            static_cast<Uint8>(UINT8_MAX * r),
            static_cast<Uint8>(UINT8_MAX * g),
            static_cast<Uint8>(UINT8_MAX * b),
            255
        };
    }

    static Color Random()
    {
        return {static_cast<float>(std::rand()) / RAND_MAX, static_cast<float>(std::rand()) / RAND_MAX, static_cast<float>(std::rand()) / RAND_MAX
        };
    }

    static const Color White;
    static const Color Black;
    static const Color Red;
    static const Color Blue;
    static const Color Green;
};