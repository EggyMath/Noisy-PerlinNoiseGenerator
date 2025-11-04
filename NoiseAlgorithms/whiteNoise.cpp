#include <cstdlib>
#include <ctime>

#include "whiteNoise.h"
/*****************************************************************************************************
 *  White Noise is a noise type that simply randomly generates a double for each point on the noise. *
 *****************************************************************************************************/

// Set pixels in noise to each be a random double
void whiteNoise::create(unsigned int width, unsigned int height, unsigned int seed) {
    // Initialize
    genericNoise::create(width, height, seed);

    // Set noise doubles to a random double
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            noise[i][j] = genericNoise::randomDouble();
        }
    }
}