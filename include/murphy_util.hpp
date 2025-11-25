//
// Created by Frank Murphy on 11/25/25.
//

#ifndef CISC220FINAL_MURPHY_UTIL_HPP
#define CISC220FINAL_MURPHY_UTIL_HPP

#include "SDL3/SDL_pixels.h"

// Clamp integer to [0, 255] and return as Uint8
Uint8 clampToByte(int v);

// Compute perceptual luminance using Rec. 709 weights
float luminance(SDL_Color c);

// Adjust brightness by a factor (<1 = darker, >1 = lighter)
SDL_Color adjustBrightness(SDL_Color c, float factor);

#endif //CISC220FINAL_MURPHY_UTIL_HPP