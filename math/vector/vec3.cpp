#include "vec3.h"
#include <cmath>

namespace math {
    // Same as vec2.cpp with additional z component and cross function
    Vec3::Vec3() : x(0), y(0), z(0) {}

    Vec3::Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float Vec3::length() const {
        return std::sqrt(x*x + y*y + z*z);
    }

    Vec3 Vec3::normalized() const {
        float len = length();

        if (len > 0) {
            return Vec3(x / len, y / len, z / len);
        }

        return Vec3(0, 0, 0);
    }

    float Vec3::dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // Compute cross product between this vector and another vector
    Vec3 Vec3::cross(const Vec3& other) const {
        return Vec3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    float Vec3::distance(const Vec3& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        float dz = z - other.z;

        return std::sqrt(dx*dx + dy*dy + dz*dz);
    }

    Vec3 Vec3::operator+(const Vec3& other) const { 
        return Vec3(x + other.x, y + other.y, z + other.z); 
    }

    Vec3 Vec3::operator-(const Vec3& other) const { 
        return Vec3(x - other.x, y - other.y, z - other.z); 
    }

    Vec3 Vec3::operator*(float scalar) const { 
        return Vec3(x * scalar, y * scalar, z * scalar); 
    }

    Vec3 Vec3::operator/(float scalar) const { 
        return Vec3(x / scalar, y / scalar, z / scalar); 
    }

    bool Vec3::operator==(const Vec3& other) const { 
        return x == other.x && y == other.y && z == other.z; 
    }
}