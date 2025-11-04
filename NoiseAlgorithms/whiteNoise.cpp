#include <cstdlib>
#include <ctime>

#include "whiteNoise.h"

void whiteNoise::create(unsigned int width, unsigned int height, unsigned int seed) {
    genericNoise::create(width, height, seed);

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            noise[i][j] = genericNoise::randomDouble();
        }
    }
}