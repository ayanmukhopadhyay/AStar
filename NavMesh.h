#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <unordered_map>
#include "Vector3.h"
#include "Polygon.h"

class NavMesh {
public:
    std::vector<Polygon> polygons;
    std::unordered_map<int, std::vector<int>> edges;

    /**
     * Constructor for the NavMesh class.
     * Initializes the navigation mesh with the provided polygons and edges.
     * 
     * @param polygons A vector of Polygon objects representing the navigation mesh.
     * @param edges An unordered_map representing the adjacency list of the navigation mesh.
     */
    NavMesh(const std::vector<Polygon>& polygons, const std::unordered_map<int, std::vector<int>>& edges)
        : polygons(polygons), edges(edges) {}

    /**
     * Heuristic function to estimate the cost from Polygon a to Polygon b.
     * Uses the Euclidean distance between the centroids of the polygons as the heuristic.
     * 
     * @param a The starting polygon.
     * @param b The goal polygon.
     * @return The heuristic cost from polygon a to polygon b.
     */
    float heuristic(const Polygon& a, const Polygon& b) {
        return a.centroid.distanceTo(b.centroid);
    }


    /**
     * A* algorithm for finding the shortest path from the start polygon to the goal polygon.
     * 
     * @param startIndex The index of the starting polygon in the polygons vector.
     * @param goalIndex The index of the goal polygon in the polygons vector.
     * @return A vector of integers representing the indices of the polygons in the shortest path.
     */
    std::vector<int> aStar(int startIndex, int goalIndex) {
        std::unordered_map<int, float> gScore;
        std::unordered_map<int, float> fScore;
        std::unordered_map<int, int> cameFrom;
        auto cmp = [&fScore](int left, int right) { return fScore[left] > fScore[right]; };
        std::priority_queue<int, std::vector<int>, decltype(cmp)> openSet(cmp);

        for (int i = 0; i < polygons.size(); ++i) {
            gScore[i] = std::numeric_limits<float>::infinity();
            fScore[i] = std::numeric_limits<float>::infinity();
        }

        gScore[startIndex] = 0;
        fScore[startIndex] = heuristic(polygons[startIndex], polygons[goalIndex]);
        openSet.push(startIndex);

        while (!openSet.empty()) {
            int current = openSet.top();
            openSet.pop();

            if (current == goalIndex) {
                return reconstructPath(cameFrom, current);
            }

            for (int neighbor : edges[current]) {
                float tentative_gScore = gScore[current] + polygons[current].centroid.distanceTo(polygons[neighbor].centroid);

                if (tentative_gScore < gScore[neighbor]) {
                    cameFrom[neighbor] = current;
                    gScore[neighbor] = tentative_gScore;
                    fScore[neighbor] = gScore[neighbor] + heuristic(polygons[neighbor], polygons[goalIndex]);
                    openSet.push(neighbor);
                }
            }
        }

        return {}; // No path found
    }

private:
    std::vector<int> reconstructPath(const std::unordered_map<int, int>& cameFrom, int current) {
        std::vector<int> path;
        path.push_back(current);
        while (cameFrom.find(current) != cameFrom.end()) {
            current = cameFrom.at(current);
            path.push_back(current);
        }
        std::reverse(path.begin(), path.end());
        return path;
    }
};
