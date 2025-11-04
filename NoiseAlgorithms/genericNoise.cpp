#include "genericNoise.h"

#include <random>
#include <cstdint>
#include <vector>
#include <sstream>

genericNoise::genericNoise() : dist(0.0, 1.0) {}

void genericNoise::create(unsigned int width, unsigned int height, unsigned int seed) {
    noise = new double*[width];
    this->width = width;
    this->height = height;

    gen.seed(seed);

    for (int i = 0; i < width; i++) {
        noise[i] = new double[height];
    }
}

std::string genericNoise::noiseToJson() {
    std::ostringstream oss;
    int width;
    int height;

    oss << "{ \"width\": " << width << ", \"height\": " << height << ", \"data\": [";

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            oss << noise[i][j];
            if (!(i == width - 1 && j == height - 1))
                oss << ",";
        }
    }

    oss << "] }";
    return oss.str();
}

genericNoise::~genericNoise() {
    for (int i = 0; i < width; i++) {
        delete[] noise[i];
    }
    delete[] noise;
}

double genericNoise::randomDouble() {
    return dist(gen);
}