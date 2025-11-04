#include "genericNoise.h"

class whiteNoise : public genericNoise {
public:
    void create(unsigned int width, unsigned int height, unsigned int seed) override;
};