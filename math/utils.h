#ifndef UTILS_H
#define UTILS_H

#include <cmath>

/**
 * @file utils.h
 * @brief Common mathematical utility functions for interpolation, clamping, and angle conversions.
 *
 * This header defines basic helper functions frequently used in graphics and procedural
 * generation algorithms, including clamp, linear interpolation (lerp), smoothstep easing,
 * and degree/radian conversion. All functions are implemented in utils.cpp.
 *
 * Part of the custom math library for Perlin noise and rendering systems.
 */


namespace math {
    // Retuns x so that it is within a specific range
    // minVal <= x <= maxVal -> return x
    // x < minVal -> return minVal
    // maxVal < x -> return maxVal
    float clamp(float x, float minVal, float maxVal);

    // Linear interpolition
    // Return the value that lies t fraction of the way between a and b (a = start, b = end, t = interpolition factor)
    float lerp(float a, float b, float t);

    // S shaped curve interpolition (eases in and out)
    float smoothstep(float edge0, float edge1, float x);

    // Convertes to radians
    float radians(float degrees);

    // Converts to degrees
    float degrees(float radians);
}

#endif