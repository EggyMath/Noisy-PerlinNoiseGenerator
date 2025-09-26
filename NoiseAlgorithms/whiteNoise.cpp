#include <cstdlib>
#include <ctime>

#include <genericNoise.h>

class whiteNoise : public genericNoise {
public:
    void create(unsigned int width, unsigned int height, unsigned int seed) override {
        genericNoise::create(width, height, seed);

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                noise[i][j] = genericNoise::randomDouble();
            }
        }
    }
};