#include "mat2.h"
#include "vec2.h"
#include <stdexcept>

namespace math {
    // Default constructor
    Mat2::Mat2() {
        m[0][0] = 1.0f; m[0][1] = 0.0f;
        m[1][0] = 0.0f; m[1][1] = 1.0f;
    }

    // Main constructor:
    // [ m00, m01 ]
    // [ m10, m11 ]
    Mat2::Mat2(float m00, float m01, float m10, float m11) {
        m[0][0] = m00; m[0][1] = m01;
        m[1][0] = m10; m[1][1] = m11;
    }

    // Matrix multiplication:
    // C = a*b
    // A = [a b] B = [e f] C = [a*e+b*g a*f+b*h]
    //     [c d]     [g h]     [c*e+d*g c*f+d*h]
    Mat2 Mat2::operator*(const Mat2& rhs) const {
        return Mat2(
            m[0][0] * rhs.m[0][0] + m[0][1] * rhs.m[1][0],
            m[0][0] * rhs.m[0][1] + m[0][1] * rhs.m[1][1],
            m[1][0] * rhs.m[0][0] + m[1][1] * rhs.m[1][0],
            m[1][0] * rhs.m[0][1] + m[1][1] * rhs.m[1][1]
        );
    }

    // Matrxi with vector multiplication:
    // A*v = c
    // A = [a b] v = (x, y) c = (a*x + b*y, c*x + d*y)
    //     [c d] 
    Vec2 Mat2::operator*(const Vec2& vec) const {
        return Vec2(
            m[0][0] * vec.x + m[0][1] * vec.y,
            m[1][0] * vec.x + m[1][1] * vec.y
        );
    }

    // Flips rows and columns
    Mat2 Mat2::transpose() const {
        return Mat2(
            m[0][0], m[1][0],
            m[0][1], m[1][1]
        );
    }

    // Determinent = ad - bc
    float Mat2::determinant() const {
        return m[0][0] * m[1][1] - m[0][1] * m[1][0];
    }

    // Computes inverse of matrix
    // A = [a b](-1) = (1/det) * [d -b]
    //     [c d]                 [-c a]
    Mat2 Mat2::inverse() const {
        float det = determinant();

        // Runtime error if determinent is 0
        if (det == 0.0f) {
            throw std::runtime_error("Matrix is singular and cannot be inverted");
        }

        float invDet = 1.0f / det;

        return Mat2(
            m[1][1] * invDet, -m[0][1] * invDet,
            -m[1][0] * invDet,  m[0][0] * invDet
        );
    }

    // Creates a 2D rotation matrix for the given angle (in radians)
    // Rotation is counter-clockwise
    Mat2 Mat2::rotation(float radians) {
        float c = std::cos(radians);
        float s = std::sin(radians);

        return Mat2(
            c, -s,
            s,  c
        );
    }
}