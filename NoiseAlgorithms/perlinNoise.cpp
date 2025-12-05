#include <iostream>
#include "perlinNoise.h"

// Constructor
perlinNoise::perlinNoise() : genericNoise() {
    perlinSeed = 0;
}

// Generate perline Noise grid
void perlinNoise::create(unsigned int width, 
                         unsigned int height, 
                         float scale, 
                         unsigned int seed, 
                         PerlinMode mode,
                         int octaves,
                         float lacunarity,
                         float gain,
                         float warpStrength) {
    // Allocate memory with base class
    genericNoise::create(width, height, seed);

    this->perlinSeed = seed;

    if (scale <= 0.0f) {
        scale = 0.0001f;
    }

    float minVal = 999.0f;
    float maxVal = -999.0f;

    for (int y = 0; y < static_cast<int>(height); y++) {
        for (int x = 0; x < static_cast<int>(width); x++) {
            float sampleX = x / scale;
            float sampleY = y / scale;

            float value = 0.0f;

            if (mode == PerlinMode::BASIC) {
                value = perlin(sampleX, sampleY);
            }
            else if (mode == PerlinMode::FBM) {
                value = fbm(sampleX, sampleY, octaves, lacunarity, gain);
            }
            else if (mode == PerlinMode::RIDGED) {
                float v = fbm(sampleX, sampleY, octaves, lacunarity, gain);
                value = 1.0f - std::abs(v * 2.0f - 1.0f);
            }
            else if (mode == PerlinMode::TURBULENCE) {
                value = std::abs(fbm(sampleX, sampleY, octaves, lacunarity, gain));
            }
            else if (mode == PerlinMode::DOMAIN_WARP) {
                float qx = fbm(sampleX + 5.2f, sampleY + 1.3f, octaves, lacunarity, gain);
                float qy = fbm(sampleX + 8.5f, sampleY + 2.8f, octaves, lacunarity, gain);

                float warpedX = sampleX + (warpStrength * qx);
                float warpedY = sampleY + (warpStrength * qy);

                value = fbm(warpedX, warpedY, octaves, lacunarity, gain);
            }
            else if (mode == PerlinMode::BILLOW) {
                float v = fbm(sampleX, sampleY, octaves, lacunarity, gain);
                value = std::abs(v * 2.0f - 1.0f);
            }

            minVal = std::min(minVal, value);
            maxVal = std::max(maxVal, value);

            noise[x][y] = value;
        }
    }

    float range = maxVal - minVal;

    for (int y = 0; y < (int)height; y++) {
        for (int x = 0; x < (int)width; x++) {
            noise[x][y] = (noise[x][y] - minVal) / range;
        }
    }

    std::cout << "PERLIN RANGE -> min: " << minVal << " max: " << maxVal << std::endl;
}

// Generates Fractal Brownian Motion (FBM) noise using Perlin noise.
float perlinNoise::fbm(float x, float y, int octaves, float lacunarity, float gain) const {
    float total = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;

    for (int i = 0; i < octaves; i++) {
        total = total + perlin(x * frequency, y * frequency) * amplitude;

        frequency = frequency * lacunarity;
        amplitude = amplitude * gain;
    }

    //return math::clamp(total, 0.0f, 1.0f);
    return total;
}

// Default: Implements Gradient-based 2D Perlin Noise (Improved style)
float perlinNoise::perlin(float x, float y) const {
    // Find the grid square of the point
    int x0 = static_cast<int>(std::floor(x));
    int x1 = x0 + 1;
    int y0 = static_cast<int>(std::floor(y));
    int y1 = y0 + 1;

    float sx = x - static_cast<float>(x0);
    float sy = y - static_cast<float>(y0);

    float n0 = dotGridGradient(x0, y0, x, y);
    float n1 = dotGridGradient(x1, y0, x, y);

    // Interpolate between the bottom-left and bottom-right values
    float ix0 = math::lerp(n0, n1, fade(sx));

    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);

    // Interpolate between the top-left and top-right values
    float ix1 = math::lerp(n0, n1, fade(sx));

    // Interpolate vertically between the top and bottom interpolated values
    float value = math::lerp(ix0, ix1, fade(sy));

    // Convert the output range from [-1, 1] to [0, 5] for image usage
    return (value + 1.0f) * 0.5f;
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
    const unsigned int PRIME1 = 0x9E3779B1;
    const unsigned int PRIME2 = 0x85EBCA77;
    const unsigned int PRIME3 = 0xC2B2AE3D;

    unsigned int hash = (unsigned int)ix;
    hash = hash ^ PRIME1;
    hash = hash + (unsigned int)iy * PRIME2;

    hash = hash ^ perlinSeed;

    hash = hash * PRIME3;
    hash = hash ^ (hash >> 15);

    float angle = (float)hash / (float)UINT32_MAX * 6.28318530718f;

    return math::Vec2(std::cos(angle), std::sin(angle));
}