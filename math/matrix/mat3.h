#ifndef MAT_3_H
#define MAT_3_H

#include "vec3.h"
#include <cmath>

namespace math {
    struct Mat3 {
        float m[3][3]; // 3 x 3 matrix

        // Default: Identity Matrix
        Mat3();

        // Main constructor
        Mat3(float m00, float m01, float m02,
            float m10, float m11, float m12,
            float m20, float m21, float m22);

        // Multiply by another 3 x 3 matrix
        Mat3 operator*(const Mat3& rhs) const;

        // Multiply by a 3 dimensional vector
        Vec3 operator*(const Vec3& vec) const;

        // Returns determinant
        float determinant() const;

        // Returns inverse matrix
        Mat3 inverse() const;

        // Rotation around Z axis
        static Mat3 rotationZ(float radians);
    };
}

#endif