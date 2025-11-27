#include "simplexNoise.h"
#include <algorithm>
#include <random>

static const math::Vec2 gradients[8] = {
    { 1, 1 }, { -1, 1 }, { 1, -1 }, { -1, -1 },
    { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }
};

simplexNoise::simplexNoise() : genericNoise() {}

void simplexNoise::create(unsigned int width,
                          unsigned int height,
                          float scale,
                          unsigned int seed,
                          int octaves,
                          float lacunarity,
                          float gain)
{
    genericNoise::create(width, height, seed);

    generatePermutation(seed);

    if (scale <= 0.0f) {
        scale = 0.001f;
    }

    float minVal = 9999.0f;
    float maxVal = -9999.0f;

    for (int y = 0; y < (int)height; y++) {
        for (int x = 0; x < (int)width; x++) {
            float nx = x / scale;
            float ny = y / scale;

            float value = fbm(nx, ny, octaves, lacunarity, gain);

            noise[x][y] = value;

            if (value < minVal) {
                minVal = value;
            }

            if (value > maxVal) {
                maxVal = value;
            }
        }
    }

    for (int y = 0; y < (int)height; y++) {
        for (int x = 0; x < (int)width; x++) {
            noise[x][y] = (noise[x][y] - minVal) / (maxVal - minVal);
        }
    }
}

float simplexNoise::fbm(float x, float y, int octaves, float lacunarity, float gain) const {
    float total = 0.0f;
    float freq = 1.0f;
    float amp = 1.0f;

    for (int i = 0; i < octaves; i++) {
        total += simplex(x * freq, y * freq) * amp;
        freq *= lacunarity;
        amp *= gain;
    }

    return total;
}

float simplexNoise::simplex(float x, float y) const {
    const float F2 = 0.366025403f; // (sqrt(3) - 1) / 2
    const float G2 = 0.211324865f; // (3 - sqrt(3)) / 6

    // Convert (x, y) to triangular grid cell coordinates
    float s = (x + y) * F2;
    int i = std::floor(x + s);
    int j = std::floor(y + s);

    // Unskew
    float t = (i + j) * G2;

    float x0 = x - (i - t);
    float y0 = y - (j - t);

    int i1;
    int j1;

    // Determine which triangle half
    if (x0 > y0) {
        i1 = 1;
        j1 = 0;
    }
    else {
        i1 = 0; 
        j1 = 1;
    }

    // Calculate three triangle corners
    float x1 = x0 - i1 + G2;
    float y1 = y0 - j1 + G2;
    float x2 = x0 - 1.0f + 2.0f * G2;
    float y2 = y0 - 1.0f + 2.0f * G2;

    int ii = i & 255;
    int jj = j & 255;

    // hash corner positions into gradiants
    int gi0 = perm[ii + perm[jj]] % 8;
    int gi1 = perm[ii + i1 + perm[jj + j1]] % 8;
    int gi2 = perm[ii + 1 + perm[jj + 1]] % 8;

    float n0, n1, n2;

    // calculate influence of each corner
    float t0 = 0.5f - x0*x0 - y0*y0;

    if (t0 < 0) {
        n0 = 0.0f;
    }
    else {
        t0 = t0 * t0;
        n0 = t0 * t0 * dot(gradients[gi0], x0, y0);
    }

    float t1 = 0.5f - x1*x1 - y1*y1;

    if (t1 < 0) {
        n1 = 0.0f;
    }
    else {
        t1 = t1 * t1;
        n1 = t1 * t1 * dot(gradients[gi1], x1, y1);
    }

    float t2 = 0.5f - x2*x2 - y2*y2;

    if (t2 < 0) {
        n2 = 0.0f;
    }
    else {
        t2 = t2 * t2;
        n2 = t2 * t2 * dot(gradients[gi2], x2, y2);
    }

    // Scale to [0, 1]
    return (70.0f * (n0 + n1 + n2)) * 0.5f + 0.5f;
}

// Creates the permutation table (pseudo-random)
void simplexNoise::generatePermutation(unsigned int seed)  {
    std::vector<int> p(256);

    for (int i = 0; i < 256; i++) {
        p[i] = i;
    }

    std::mt19937 rng(seed);
    std::shuffle(p.begin(), p.end(), rng);

    for (int i = 0; i < 512; i++) {
        perm[i] = p[i & 255];
    }
}

float simplexNoise::dot(const math::Vec2& g, float x, float y) const {
    return g.x * x + g.y * y;
}

