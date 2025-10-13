#include "vec2.h"
#include <cmath>

namespace math {
    Vec2::Vec2() : x(0), y(0) {}

    Vec2::Vec2(float x_, float y_) : x(x_), y(y_) {}

    // Length = sqrt(x^2 + y^2)
    float Vec2::length() const {
        return std::sqrt(x * x + y * y);
    }

    // Return the unit vector for the given vector
    Vec2 Vec2::normalized() const {
        float len = length();

        if (len > 0) {
            return Vec2(x / len, y / len);
        }

        return Vec2(0, 0);
    }

    // Dot product: x1x2 + y1y2
    float Vec2::dot(const Vec2& other) const {
        return x * other.x + y * other.y;
    }

    // Uses distance formula to get the distance between two vectors
    float Vec2::distance(const Vec2& other) const {
        float dx = x - other.x;
        float dy = y - other.y;

        return std::sqrt(dx * dx + dy * dy);
    }

    // reflects the vector about a given normal vector
    Vec2 Vec2::reflect(const Vec2& normal) const  {
        float dotProd = this->dot(normal);
        Vec2 projection = normal * dotProd * 2.0f;
        Vec2 reflected = *this - projection;
        return reflected;
    }

    // Adds two vectors together: sumV = (x1 + x2, y1 + y2)
    Vec2 Vec2::operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }

    // Subtract two vectors: diffV = (x1 - x2, y1 - y2)
    Vec2 Vec2::operator-(const Vec2& other) const { 
        return Vec2(x - other.x, y - other.y); 
    }

    // Multiplies the vector by a scaler
    Vec2 Vec2::operator*(float scalar) const { 
        return Vec2(x * scalar, y * scalar); 
    }

    // Divides the vector by a scaler
    Vec2 Vec2::operator/(float scalar) const { 
        return Vec2(x / scalar, y / scalar); 
    }

    // deterines if two vectors are equal (their x and y is equal)
    bool Vec2::operator==(const Vec2& other) const { 
        return x == other.x && y == other.y; 
    }
}