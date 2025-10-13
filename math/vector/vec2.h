#ifndef VEC_2_H
#define VEC_2_h

namespace math {

    /**
     * @struct Vec2
     * @brief Represents a 2-dimensional vector.
     */
    struct Vec2 {
        float x; // X component
        float y; // Y component

        /* Default constructor (0,0) */
        Vec2();

        // Construct using given x and y components
        Vec2(float x_, float y_);

        // Computes the length of the vector
        float length() const;

        // Returns the normalized (unit) vector for the given vector
        Vec2 normalized() const;

        // Computed dot product with nother vector
        float dot(const Vec2& other) const;

        // Returns the distance between this vector and the other vector
        float distance(const Vec2& other) const;

        // Reflects the vector around a normal
        Vec2 reflect(const Vec2& normal) const;

        // Overloaded operators
        Vec2 operator+(const Vec2& other) const;
        Vec2 operator-(const Vec2& other) const;
        Vec2 operator*(float scalar) const;
        Vec2 operator/(float scalar) const;
        bool operator==(const Vec2& other) const;
    };
}

#endif