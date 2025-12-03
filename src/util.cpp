//
// Created by Frank Murphy on 11/25/25.
//

#include "../include/util.hpp"

#include <string>

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

char toHexDigit(const int n) {
    if (n < 0 || n > 15) return '?';
    return (n < 10) ? ('0' + n) : ('A' + (n - 10));
}

std::string convertToLower(std::string original) {
    // Convert the string to lowercase
    std::transform(original.begin(), original.end(), original.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return original;
}
std::string removeSpaces(std::string original) {
    original.erase(std::remove_if(original.begin(), original.end(), ::isspace), original.end());
    return original;
}
