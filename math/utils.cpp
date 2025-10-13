#include "utils.h"
#include "constants.h"

namespace math {
    float clamp(float x, float minVal, float maxVal) {
        return std::fmax(minVal, std::fmin(x, maxVal));
    }

    // Linear interpolition
    float lerp(float a, float b, float t) {
        return a + t * (b - a);
    }

    // S shaped curve interpolition (eases in and out)
    float smoothstep(float e0, float e1, float x) {
        x = clamp((x - e0) / (e1 - e0), 0.0f, 1.0f);
        return x * x * (3 - 2 * x);
    }

    // Convertes to radians
    float radians(float deg) {
        return deg * (PI / 180.0f);
    }

    // Converts to degrees
    float degrees(float rad) {
        return rad * (180.0f / PI);
    }
}