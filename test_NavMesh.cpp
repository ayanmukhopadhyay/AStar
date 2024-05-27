#include <iostream>
#include <unordered_map>
#include <vector>
#include "NavMesh.h"
#include "Polygon.h"
#include "Vector3.h"
#include "utils.cpp"


void test_simple_grid() {
    // Define vertices
    std::vector<Vector3> vertices1 = {Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(1, 0, 1), Vector3(0, 0, 1)};
    std::vector<Vector3> vertices2 = {Vector3(1, 0, 0), Vector3(2, 0, 0), Vector3(2, 0, 1), Vector3(1, 0, 1)};
    std::vector<Vector3> vertices3 = {Vector3(0, 0, 1), Vector3(1, 0, 1), Vector3(1, 0, 2), Vector3(0, 0, 2)};
    std::vector<Vector3> vertices4 = {Vector3(1, 0, 1), Vector3(2, 0, 1), Vector3(2, 0, 2), Vector3(1, 0, 2)};

    // Create polygons
    Polygon poly1(vertices1);
    Polygon poly2(vertices2);
    Polygon poly3(vertices3);
    Polygon poly4(vertices4);

    // Define navmesh
    std::vector<Polygon> polygons = {poly1, poly2, poly3, poly4};
    std::unordered_map<int, std::vector<int>> edges = {
        {0, {1, 2}},
        {1, {0, 3}},
        {2, {0, 3}},
        {3, {1, 2}}
    };

    NavMesh navMesh(polygons, edges);

    // Find path
    int start = 0;
    int goal = 3;
    std::vector<int> path = navMesh.aStar(start, goal);

    // Print path
    if (!path.empty()) {
        std::cout << "Path found:\n";
        for (int index : path) {
            std::cout << polygons[index] << "\n";
        }
    } else {
        std::cout << "No path found.\n";
    }
}


void test_10x10x10_grid() {
    int size = 10; // Size of the grid
    std::vector<Polygon> polygons = createGrid(size);
    std::unordered_map<int, std::vector<int>> edges = createEdges(size);

    // Add some obstacles by removing edges
    std::vector<std::pair<int, int>> obstacles = {
        {3, 4}, {3, 5}, {4, 5}, {5, 4}, {6, 5},
        {12, 13}, {12, 14}, {13, 14}, {14, 13},
        {45, 46}, {46, 47}, {47, 46}, {48, 47}
    };

    for (auto obstacle : obstacles) {
        edges[obstacle.first].erase(std::remove(edges[obstacle.first].begin(), edges[obstacle.first].end(), obstacle.second), edges[obstacle.first].end());
        edges[obstacle.second].erase(std::remove(edges[obstacle.second].begin(), edges[obstacle.second].end(), obstacle.first), edges[obstacle.second].end());
    }

    NavMesh navMesh(polygons, edges);

    // Define start and goal points
    int start = 0; // Starting point (0, 0, 0)
    int goal = size * size * size - 1; // Goal point (9, 9, 9)
    std::vector<int> path = navMesh.aStar(start, goal);

    // Print the resulting path
    if (!path.empty()) {
        std::cout << "Path found:\n";
        for (int index : path) {
            std::cout << "Polygon index: " << index << ", Centroid: " << polygons[index].centroid << "\n";
        }
    } else {
        std::cout << "No path found.\n";
    }
}


int main() {
    test_simple_grid();
    test_10x10x10_grid();
    return 0;
}
