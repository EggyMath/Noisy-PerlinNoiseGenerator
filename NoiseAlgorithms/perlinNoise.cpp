#include "perlinNoise.h"

// Constructor
perlinNoise::perlinNoise() : genericNoise() {}

void perlinNoise::create(unsigned int width, unsigned int height, float scale, unsigned int seed) {
    // Allocate memeory with base class
    genericNoise::create(width, height, seed);

    if (scale <= 0.0f) {
        scale = 0.0001f;
    }

    for (int y = 0; y < static_cast<int>(height); y++) {
        for (int x = 0; x < static_cast<int>(width); x++) {
            float sampleX = x / scale;
            float sampleY = y / scale;

            noise[x][y] = perlin(sampleX, sampleY);
        }
    }
}

// This must be an equation that satisfies the following necessary constraints for smoothing:
// f(0) = 0
// f(1) = 1
// f'(0) = 0 
// f'(1) = 0
// f''(0) = 0 
// f''(1) = 0​
float perlinNoise::fade(float t) const {
    // 6t^5 - 15t^4 + 10t^3
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float perlinNoise::dotGridGradient(int ix, int iy, float x, float y) const {
    math::Vec2 gradient = randomGradient(ix, iy);

    float dx = x - static_cast<float>(ix);
    float dy = y - static_cast<float>(iy);

    return (dx * gradient.x + dy * gradient.y);
}

math::Vec2 perlinNoise::randomGradient(int ix, int iy) const {
    const unsigned int PRIME1 = 3284157443u;
    const unsigned int PRIME2 = 1911520717u;
    const unsigned int PRIME3 = 2048419325u;

    const unsigned int BIT_COUNT = 8 * sizeof(unsigned int);
    const unsigned int SHIFT = BIT_COUNT / 2;

    unsigned int hashX = ix;
    hashX = hashX * PRIME1;

    unsigned int hashY = iy;
    hashY = hashX ^ (hashX << SHIFT) | (hashX >> (BIT_COUNT - SHIFT));
    hashY = hashY * PRIME2;

    unsigned int finalHash = hashX ^ ((hashY << SHIFT) | (hashY >> (BIT_COUNT - SHIFT)));
    finalHash = finalHash * PRIME3;

    float angle = (float)finalHash / (float)UINT32_MAX * 6.28318530718f;

    return math::Vec2(std::cos(angle), std::sin(angle));
}