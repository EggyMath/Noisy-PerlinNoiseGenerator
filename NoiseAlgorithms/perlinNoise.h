#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include "genericNoise.h"
#include "math/vector/vec2.h"
#include "math/utils.h"

enum class PerlinMode {
    BASIC,
    FBM,
    RIDGED,
    TURBULENCE,
    DOMAIN_WARP
};

// inherits from genericNoise
class perlinNoise : public genericNoise {
    public:
        /**
         * @brief Constructs a new perlinNoise object.
         *
         * Uses the base genericNoise constructor to initialize
         * the random number engine and internal state.
         */
        perlinNoise();

        /**
         * @brief Generates a Perlin noise map and stores it in noise[][].
         *
         * This function creates a smooth, coherent noise field
         * using gradient-based Perlin noise. The "scale" determines
         * the frequency of the noise (larger = smoother, smaller = more detailed).
         * 
         *  - BASIC: Uses a single octave of classic Perlin noise
         *    (smooth, blobby, low-detail patterns)
         *  - FBM: Uses Fractal Brownian Motion (multiple layered Perlin
         *    octaves) to create richer, more natural-looking detail
         *  - RIDGED: Produces sharp, mountainous ridge-like patterns by
         *    inverting and reshaping FBM output (ideal for terrain)
         *  - TURBULENCE: Uses the absolute value of stacked Perlin noise to create
         *    chaotic, high-contrast patterns (useful for fire, marble, etc.)
         *  - DOMAIN_WARP: Warps the input coordinates using additional noise
         *    before sampling, producing highly organic, swirling,
         *    and distorted patterns
         *
         * When using FBM mode, additional parameters control how the octaves
         * are layered together:
         *  - octaves:      Number of noise layers to combine
         *  - lacunarity:   Frequency multiplier per octave (usually ~2.0)
         *  - gain:         Amplitude multiplier per octave (usually ~0.5)
         * 
         * For DOMAIN_WARP mode, an additional parameter controls how strongly
         * the coordinates are distorted:
         *  - warpStrength : Intensity of the coordinate warping effect
         * 
         * @param width The width of the noise map in pixels
         * @param height The height of the noise map in pixels
         * @param scale Controls the frequency of the noise (larger = smoother)
         * @param seed Seed used for deterministic randomness
         * @param mode Selects BASIC Perlin or FBM noise generation mode
         * @param octaves Number of octaves used when mode == PerlinMode::FBM
         * @param lacunarity Frequency multiplier used in FBM
         * @param gain Amplitude scaling factor used in FBM
         * @param warpStrength  Strength of coordinate warping (used in DOMAIN_WARP)
         */
        void create(unsigned int width,
            unsigned int height,
            float scale,
            unsigned int seed,
            PerlinMode mode = PerlinMode::BASIC,
            int octaves = 5,
            float lacunarity = 2.0f,
            float gain = 0.5f,
            float warpStrength = 10.0f);

        /**
         * @brief Generates Fractal Brownian Motion (FBM) noise using Perlin noise.
         * 
         * This function layers (stacks) multiple octaves of Perlin noise at
         * increasing frequencies and decreasing amplitudes. Each successive
         * octave adds finer detail while contributing less to the final value.
         * 
         * This creates a more complex, natural-looking pattern compared to
         * standard Perlin noise, which only contains a single frequency layer.
         * 
         * @param x Continuous x coordinate
         * @param y Continuous y coordinate
         * @param octaves Number of Perlin layers to combine
         * @param lacunarity Controls how quickly frequency increases per octave
         * @param gain Controls how quickly amplitude decreases per octave
         * @return A smooth noise value in the range [0, 1]
         */
        float fbm(float x, float y, int octaves, float lacunarity, float gain) const;
    private:
        /**
         * @brief Computes the Perlin noise value at a given (x, y) coordinate.
         *
         * Determines the surrounding grid cell, retrieves the local gradient
         * vectors, computes dot products with distance vectors, and interpolates
         * the results using a fade curve.
         *
         * @param x The x coordinate in noise space
         * @param y The y coordinate in noise space
         * @return A smooth noise value in the range [0, 1]
         */
        float perlin(float x, float y) const;

        /**
         * @brief Smooths a value using Perlin's quintic fade curve.
         * 
         * It is used during interpolation to ensure that transitions between
         * grid points are smooth in both value and slope. This eliminates hard
         * edges and visible seams in the generated noise.
         * 
         * @param t A value in the range [0, 1]
         * @return Smoothed value in the range [0, 1]
         */
        float fade(float t) const;

        /**
         * @brief Computes the dot product between a grid gradient and a
         *        distance vector from that grid point to (x, y).
         *
         * @param ix Integer grid x coordinate
         * @param iy Integer grid y coordinate
         * @param x Continuous x coordinate
         * @param y Continuous y coordinate
         * @return Dot product used for interpolation
         */
        float dotGridGradient(int ix, int iy, float x, float y) const;

        /**
         * @brief Generates a deterministic unit gradient vector
         *        for a given grid coordinate.
         *
         * Uses a hash function based on (ix, iy) to produce a consistent
         * direction for each grid point.
         *
         * @param ix Integer grid x coordinate
         * @param iy Integer grid y coordinate
         * @return A 2D unit vector representing the gradient direction
         */
        math::Vec2 randomGradient(int ix, int iy) const;
};

#endif