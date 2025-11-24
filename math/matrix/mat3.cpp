#include "mat3.h"
#include "vec3.h"
#include <stdexcept>

namespace math {
    // Default
    Mat3::Mat3() {
        m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
        m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
        m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;
    }

    // Main
    Mat3::Mat3(float m00, float m01, float m02,
               float m10, float m11, float m12,
               float m20, float m21, float m22) {
        m[0][0] = m00;  m[0][1] = m01;  m[0][2] = m02;
        m[1][0] = m10;  m[1][1] = m11;  m[1][2] = m12;
        m[2][0] = m20;  m[2][1] = m21;  m[2][2] = m22;
    }

    // Matrix multiplication
    Mat3 Mat3::operator*(const Mat3& rhs) const  {
        Mat3 result;

        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                result.m[row][col] =
                    m[row][0] * rhs.m[0][col] +
                    m[row][1] * rhs.m[1][col] +
                    m[row][2] * rhs.m[2][col];
            }
        }

        return result;
    }

    // Matrix * vector
    Vec3 Mat3::operator*(const Vec3& vec) const {
        return Vec3(
            m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z,
            m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z,
            m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z
        );
    }

    // Calculate determinent
    float Mat3::determinant() const {
        const float a = m[0][0];
        const float b = m[0][1];
        const float c = m[0][2];

        const float d = m[1][0];
        const float e = m[1][1];
        const float f = m[1][2];

        const float g = m[2][0];
        const float h = m[2][1];
        const float i = m[2][2];

        // a(ei − fh) − b(di − fg) + c(dh − eg)
        float det = (a * (e * i - f * h))
                  - (b * (d * i - f * g))
                  + (c * (d * h - e * g));

        return det;
    }

    // Calculates the inverse matrix M^-1
    Mat3 Mat3::inverse() const {
        float det = determinant();

        // Determinent cannot be 0, otherwise it is not invertable
        if (std::fabs(det) < 1e-6f) {
            throw std::runtime_error("Mat3::inverse() - Matrix is not invertible (det = 0)");
        }

        float invDet = 1.0f / det;

        return Mat3(
            (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * invDet,
            -(m[0][1] * m[2][2] - m[0][2] * m[2][1]) * invDet,
            (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invDet,

            -(m[1][0] * m[2][2] - m[1][2] * m[2][0]) * invDet,
            (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invDet,
            -(m[0][0] * m[1][2] - m[0][2] * m[1][0]) * invDet,

            (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * invDet,
            -(m[0][0] * m[2][1] - m[0][1] * m[2][0]) * invDet,
            (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * invDet
        );
    }

    // Rotates the matrix around the z-axis
    Mat3 Mat3::rotationZ(float radians) {
        float c = std::cos(radians);
        float s = std::sin(radians);

        return Mat3(
            c, -s, 0,
            s,  c, 0,
            0,  0, 1
        );
    }
}