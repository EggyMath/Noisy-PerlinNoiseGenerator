#ifndef MAT_4_H
#define MAT_4_H

#include <cmath>

namespace math {
    struct Mat4 {
        float m[4][4]; // 4 x 4 matrix

        // Default: Identity Matrix
        Mat4();

        // Main constructor
        Mat4(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33);

        // Multiply by another 4 x 4 matrix
        Mat4 operator*(const Mat4& rhs) const;

        // Returns transposed matrix
        Mat4 transpose() const;
    };
}

#endif