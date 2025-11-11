#include "genericNoise.h"

#include <random>
#include <cstdint>
#include <vector>
#include <sstream>

genericNoise::genericNoise() : dist(0.0, 1.0) {}

// Modified function to create noise with cutoff
genericNoise::genericNoise(const genericNoise& startNoise, double cutOff) {
    // Initialize
    noise = new double*[startNoise.width];
    this->width = startNoise.width;
    this->height = startNoise.height;

    // Set noise values to extremes
    for (int i = 0; i < width; i++) {
        noise[i] = new double[height];
        for (int j = 0; j < height; j++) {
            noise[i][j] = startNoise.noise[i][j] > cutOff ? 0.99 : 0.0;
        }
    }
}

// Default function to create noise
void genericNoise::create(unsigned int width, unsigned int height, unsigned int seed) {
    // Initialize
    noise = new double*[width];
    this->width = width;
    this->height = height;

    // Set seed for random
    gen.seed(seed);

    // Allocate memory for noise pointers
    for (int i = 0; i < width; i++) {
        noise[i] = new double[height];
    }
}

// Convert noise information to a json script as a string for image sending
std::string genericNoise::noiseToJson() {
    std::ostringstream oss;

    // Set default image parameters
    oss << "{ \"width\": " << width << ", \"height\": " << height << ", \"data\": [";

    // Fill data with pixel information
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            oss << noise[i][j];
            if (!(i == width - 1 && j == height - 1))
                oss << ",";
        }
    }

    // Return json
    oss << "] }";
    return oss.str();
}

// Deallocate memory used by pointers
genericNoise::~genericNoise() {
    for (int i = 0; i < width; i++) {
        delete[] noise[i];
    }
    delete[] noise;
}

// Create random double for any need
double genericNoise::randomDouble() {
    return dist(gen);
}