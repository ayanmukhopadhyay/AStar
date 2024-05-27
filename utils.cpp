#include <unordered_map>
#include <vector>
#include "Vector3.h"
#include "Polygon.h"


// * Helper function to create a grid of polygons
std::vector<Polygon> createGrid(int size) {
    std::vector<Polygon> polygons;
    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            for (int z = 0; z < size; ++z) {
                std::vector<Vector3> vertices = {
                    Vector3(x, y, z),
                    Vector3(x + 1, y, z),
                    Vector3(x + 1, y + 1, z),
                    Vector3(x, y + 1, z),
                    Vector3(x, y, z + 1),
                    Vector3(x + 1, y, z + 1),
                    Vector3(x + 1, y + 1, z + 1),
                    Vector3(x, y + 1, z + 1)
                };
                polygons.emplace_back(vertices);
            }
        }
    }
    return polygons;
}


// * Helper function to create edges for the grid
std::unordered_map<int, std::vector<int>> createEdges(int size) {
    std::unordered_map<int, std::vector<int>> edges;
    auto index = [size](int x, int y, int z) { return x * size * size + y * size + z; };
    
    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            for (int z = 0; z < size; ++z) {
                int current = index(x, y, z);
                std::vector<int> neighbors;
                if (x > 0) neighbors.push_back(index(x - 1, y, z));          // left
                if (x < size - 1) neighbors.push_back(index(x + 1, y, z));   // right
                if (y > 0) neighbors.push_back(index(x, y - 1, z));          // down
                if (y < size - 1) neighbors.push_back(index(x, y + 1, z));   // up
                if (z > 0) neighbors.push_back(index(x, y, z - 1));          // back
                if (z < size - 1) neighbors.push_back(index(x, y, z + 1));   // front
                edges[current] = neighbors;
            }
        }
    }
    return edges;
}