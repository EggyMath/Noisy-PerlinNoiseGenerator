#ifndef MAT_2_H
#define MAT_2_H

#include "vec2.h"
#include <cmath>

namespace math {
    struct Mat2 {
        float m[2][2]; // 2 x 2 matrix

        // Default: Identity Matrix
        Mat2();

        // Main constructor
        Mat2(float m00, float m01,
            float m10, float m11);

        // Multiply this matrix by another 2 * 2 matrix
        Mat2 operator*(const Mat2& rhs) const;

        // Multiply by 2 * 2 vector
        Vec2 operator*(const Vec2& vec) const;

        // Returns transposed matrix
        Mat2 transpose() const;

        // Returns determinent
        float determinant() const;

        // Returns inverse matrix
        Mat2 inverse() const;

        // Create a 2D rotation matrix
        static Mat2 rotation(float radians);
    };
}

#endif