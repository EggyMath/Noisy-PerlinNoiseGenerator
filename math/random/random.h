#ifndef RANDOM_H
#define RANDOM_H

namespace math {
    // Simple deterministic "hash" function
    inline int permute(int x) {
        // Scramble the bits of the input integer
        x = x * 34 + 1;

        // Keep the result in the range [0, 255]
        // by taking only the last 8 bits
        x = x & 255;

        return x;
    }
}

#endif