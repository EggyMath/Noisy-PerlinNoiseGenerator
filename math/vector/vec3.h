#ifndef VEC_3_H
#define VEC_3_H

namespace math {
    struct Vec3 {
        float x; // X component
        float y; // Y component
        float z; // Z component

        // Default constructor (0,0)
        Vec3();

        // Construct using given x, y, and z components
        Vec3(float x_, float y_, float z_);

        // Computes the length of the vector
        float length() const;

        // Returns the normalized (unit) vector for the given vector
        Vec3 normalized() const;

        // Computed dot product with nother vector
        float dot(const Vec3& other) const;

        // Returns the distance between this vector and the other vector
        float distance(const Vec3& other) const;

        // Reflects the vector around a normal
        Vec3 reflect(const Vec3& normal) const;

        // Computes cross product with another vector
        Vec3 cross(const Vec3& other) const;

        // Overloaded operators
        Vec3 operator+(const Vec3& other) const;
        Vec3 operator-(const Vec3& other) const;
        Vec3 operator*(float scalar) const;
        Vec3 operator/(float scalar) const;
        bool operator==(const Vec3& other) const;
    };
}

#endif