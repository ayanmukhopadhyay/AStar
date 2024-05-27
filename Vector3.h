#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>
#include <cmath>

class Vector3 {
public:
    float x, y, z;

    /**
     * Constructor for Vector3.
     * Initializes the vector with the provided values or defaults to 0.
     * 
     * @param x The x-coordinate of the vector (default is 0).
     * @param y The y-coordinate of the vector (default is 0).
     * @param z The z-coordinate of the vector (default is 0).
     */
    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    /**
     * Calculates the Euclidean distance between this vector and another vector.
     * 
     * @param other The other vector to which the distance is calculated.
     * @return The Euclidean distance between this vector and the other vector.
     */
    float distanceTo(const Vector3& other) const {
        return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2) + std::pow(z - other.z, 2));
    }

    /**
     * Overloads the << operator for outputting a Vector3 object.
     * Allows printing the vector in the format: Vector3(x, y, z).
     * 
     * @param os The output stream object.
     * @param vec The vector to be printed.
     * @return The output stream object.
     */
    friend std::ostream& operator<<(std::ostream& os, const Vector3& vec) {
        os << "Vector3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        return os;
    }
};

#endif // VECTOR3_H
