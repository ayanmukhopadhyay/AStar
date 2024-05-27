#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include "Vector3.h"

class Polygon {
public:
    std::vector<Vector3> vertices;
    Vector3 centroid;

    /**
     * Constructor for the Polygon class.
     * Initializes the polygon with the provided vertices and computes the centroid.
     * 
     * @param vertices A vector of Vector3 objects representing the vertices of the polygon.
     */
    Polygon(const std::vector<Vector3>& vertices) : vertices(vertices) {
        computeCentroid();
    }

    void computeCentroid() {
        float x = 0, y = 0, z = 0;
        for (const auto& vertex : vertices) {
            x += vertex.x;
            y += vertex.y;
            z += vertex.z;
        }
        x /= vertices.size();
        y /= vertices.size();
        z /= vertices.size();
        centroid = Vector3(x, y, z);
    }

    friend std::ostream& operator<<(std::ostream& os, const Polygon& poly) {
        os << "Polygon(centroid: " << poly.centroid << ")";
        return os;
    }
};

#endif // POLYGON_H
