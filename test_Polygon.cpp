#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <queue>
#include <unordered_map>
#include "Vector3.h"
#include "Polygon.h"


void testPolygon() {
    std::vector<Vector3> vertices = {
        Vector3(0, 0, 6),
        Vector3(1, 0, 4),
        Vector3(1, 1, 6),
        Vector3(0, 1, 4)
    };

    Polygon poly(vertices);

    std::cout << "Testing Polygon class:" << std::endl;
    std::cout << "Polygon vertices: ";
    for (const auto& vertex : vertices) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;

    std::cout << "Polygon centroid: " << poly.centroid << std::endl;
}


int main() {
    testPolygon();
    return 0;
}
