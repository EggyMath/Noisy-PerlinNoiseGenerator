#ifndef SIMPLEX_NOISE_H
#define SIMPLEX_NOISE_H

#include "genericNoise.h"
#include "math/vector/vec2.h"
#include <vector>

/**
 * @class simplexNoise
 * @brief Generates 2D Simplex Noise and stores it in noise[][].
 *
 * Simplex noise is an improved alternative to classic Perlin noise invented
 * by Ken Perlin. It reduces visible grid artifacts, performs better in higher
 * dimensions, and produces smoother, more natural features when layered.
 * 
 * Compared to Perlin:
 *  - Fewer directional artifacts
 *  - Better performance
 *  - More natural gradient distribution
 */
class simplexNoise : public genericNoise {
    public:
        /**
         * @brief Constructs a Simplex noise generator.
         *
         * Initializes the base genericNoise class. The permutation table
         * is generated later when create() is called with a seed.
         */
        simplexNoise();

        /**
         * @brief Generates a 2D Simplex noise field and stores it in noise[][].
         *
         * Each pixel is sampled using Simplex noise and optionally combined
         * across multiple octaves to produce layered detail (FBM).
         *
         * Higher octaves introduce finer detail, while lacunarity and gain
         * control frequency and amplitude changes between layers.
         *
         * @param width Width of the noise map in pixels
         * @param height Height of the noise map in pixels
         * @param scale Controls the frequency of the noise (larger = smoother, smaller = more detailed)
         * @param seed Seed used to randomize the permutation table
         * @param octaves Number of noise layers combined (default: 5)
         * @param lacunarity Frequency multiplier per octave (default: 2.0)
         * @param gain Amplitude multiplier per octave (default: 0.5)
         */
        void create(unsigned int width,
                    unsigned int height,
                    float scale,
                    unsigned int seed,
                    int octaves = 5,
                    float lacunarity = 2.0f,
                    float gain = 0.5f);
    private:
        /**
         * @brief Computes a single 2D Simplex noise value at (x, y).
         *
         * Implements the classic 2D Simplex algorithm using skewing,
         * unskewing and gradient dot products.
         *
         * @param x Sample x-coordinate
         * @param y Sample y-coordinate
         * @return Noise value in roughly the range [0, 1]
         */
        float simplex(float x, float y) const;

        /**
         * @brief Fractal Brownian Motion using Simplex Noise.
         *
         * Multiple layers of simplex noise are combined to create
         * more natural, detailed structures.
         *
         * @param x Sample x-coordinate
         * @param y Sample y-coordinate
         * @param octaves Number of noise layers
         * @param lacunarity Frequency multiplier per layer
         * @param gain Amplitude multiplier per layer
         * @return Combined FBM noise value
         */
        float fbm(float x, float y, int octaves, float lacunarity, float gain) const;

        /**
         * @brief Permutation table used for gradient lookup.
         *
         * The table is generated using the supplied seed and
         * repeated twice for hash wrapping.
         */
        int perm[512];

        /**
         * @brief Generates the permutation table based on a seed.
         *
         * Ensures same seed always produces the same noise pattern.
         *
         * @param seed The random seed
         */
        void generatePermutation(unsigned int seed);

        /**
         * @brief Computes a dot product between a gradient vector and an offset.
         *
         * @param g Gradient vector
         * @param x Offset x from grid point
         * @param y Offset y from grid point
         * @return Dot product result
         */
        float dot(const math::Vec2& g, float x, float y) const;
};

#endif