//
// Created by Frank Murphy on 11/25/25.
//

#include "../include/murphy_util.hpp"

#include "SDL3/SDL_pixels.h"

Uint8 clampToByte(const int v)
{
    if (v < 0) return 0;
    if (v > 255) return 255;
    return static_cast<Uint8>(v);
}

float luminance(const SDL_Color c) {
    // Rec. 709 weights
    return 0.2126f * c.r + 0.7152f * c.g + 0.0722f * c.b;
}

SDL_Color adjustBrightness(const SDL_Color c, const float factor) {
    // factor < 1.0 -> darker, > 1.0 -> lighter
    SDL_Color out;
    out.r = clampToByte(static_cast<int>(c.r * factor));
    out.g = clampToByte(static_cast<int>(c.g * factor));
    out.b = clampToByte(static_cast<int>(c.b * factor));
    out.a = c.a;
    return out;
}