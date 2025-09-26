#include <random>

class genericNoise {
private:
    std::mt19937 gen;
    std::uniform_real_distribution<double> dist;

public:
    double** noise;
    unsigned int seed;
    int width;
    int height;

    genericNoise() : dist(0.0, 1.0) {}

    virtual void create(unsigned int width, unsigned int height, unsigned int seed) {
        noise = new double*[width];
        this->width = width;
        this->height = height;

        gen.seed(seed);

        for (int i = 0; i < width; i++) {
            noise[i] = new double[height];
        }
    }

    ~genericNoise() {
        for (int i = 0; i < width; i++) {
            delete[] noise[i];
        }
        delete[] noise;
    }

    double randomDouble() {
        return dist(gen);
    }
};