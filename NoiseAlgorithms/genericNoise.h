#include <random>

#ifndef GENERICNOISE
#define GENERICNOISE

class genericNoise {
protected:
    std::mt19937 gen;
    std::uniform_real_distribution<double> dist;

public:
    double** noise;
    unsigned int seed;
    int width;
    int height;

    genericNoise();
    genericNoise(const genericNoise& startNoise, double cutOff);

    virtual void create(unsigned int width, unsigned int height, unsigned int seed);

    std::string noiseToJson();

    ~genericNoise();

    double randomDouble();
};

#endif