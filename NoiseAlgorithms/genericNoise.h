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

    virtual void create(unsigned int width, unsigned int height, unsigned int seed);

    ~genericNoise();

    double randomDouble();
};