#include <random>
#include <cstdint>
#include <vector>

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

    std::vector<unsigned char> getGrayscaleImageData() {
        std::vector<unsigned char> pixels(width * height * 4);

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                //Set colors
                pixels[(j * width + i) * 4 + 0] = static_cast<unsigned char>(noise[i][j] * 255); //RED
                pixels[(j * width + i) * 4 + 0] = static_cast<unsigned char>(noise[i][j] * 255); //GREEN
                pixels[(j * width + i) * 4 + 0] = static_cast<unsigned char>(noise[i][j] * 255); //BLUE
                pixels[(j * width + i) * 4 + 0] = 255; //Alpha (opaque)
            }
        }

        return pixels;
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