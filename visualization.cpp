#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/glu.h>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <queue>
#include "Vector3.h"
#include "Polygon.h"
#include "NavMesh.h"
#include "utils.cpp"

// Function prototypes
void initOpenGL();
void drawCube(const Vector3& centroid, float size);
void display(const std::vector<Polygon>& polygons, const std::vector<int>& path);

// Main function
int main() {
    // Create an SFML window
    sf::Window window(sf::VideoMode(800, 600), "3D Path Visualization", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);

    // Initialize OpenGL settings
    initOpenGL();

    // Define the grid and path for visualization
    int size = 10;
    std::vector<Polygon> polygons = createGrid(size);
    std::unordered_map<int, std::vector<int>> edges = createEdges(size);

    // Add obstacles and create the NavMesh
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
    int start = 0; // Starting point
    int goal = size * size * size - 1; // Goal point
    std::vector<int> path = navMesh.aStar(start, goal);

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Clear the window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render the grid and path
        display(polygons, path);

        // Swap buffers
        window.display();
    }

    return 0;
}

void initOpenGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);              // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);               // Set the type of depth-test
    glShadeModel(GL_SMOOTH);              // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Nice perspective corrections

    glMatrixMode(GL_PROJECTION);          // Switch to the projection matrix
    glLoadIdentity();                     // Reset the projection matrix
    gluPerspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f); // Set perspective projection
    glMatrixMode(GL_MODELVIEW);           // Switch back to the model-view matrix
}

void drawCube(const Vector3& centroid, float size) {
    float halfSize = size / 2.0f; // Half the size of the cube

    glBegin(GL_QUADS); // Start drawing a quad
    // Front face
    glVertex3f(centroid.x - halfSize, centroid.y - halfSize, centroid.z + halfSize);
    glVertex3f(centroid.x + halfSize, centroid.y - halfSize, centroid.z + halfSize);
    glVertex3f(centroid.x + halfSize, centroid.y + halfSize, centroid.z + halfSize);
    glVertex3f(centroid.x - halfSize, centroid.y + halfSize, centroid.z + halfSize);
    // Back face
    glVertex3f(centroid.x - halfSize, centroid.y - halfSize, centroid.z - halfSize);
    glVertex3f(centroid.x - halfSize, centroid.y + halfSize, centroid.z - halfSize);
    glVertex3f(centroid.x + halfSize, centroid.y + halfSize, centroid.z - halfSize);
    glVertex3f(centroid.x + halfSize, centroid.y - halfSize, centroid.z - halfSize);
    // Top face
    glVertex3f(centroid.x - halfSize, centroid.y + halfSize, centroid.z - halfSize);
    glVertex3f(centroid.x - halfSize, centroid.y + halfSize, centroid.z + halfSize);
    glVertex3f(centroid.x + halfSize, centroid.y + halfSize, centroid.z + halfSize);
    glVertex3f(centroid.x + halfSize, centroid.y + halfSize, centroid.z - halfSize);
    // Bottom face
    glVertex3f(centroid.x - halfSize, centroid.y - halfSize, centroid.z - halfSize);
    glVertex3f(centroid.x + halfSize, centroid.y - halfSize, centroid.z - halfSize);
    glVertex3f(centroid.x + halfSize, centroid.y - halfSize, centroid.z + halfSize);
    glVertex3f(centroid.x - halfSize, centroid.y - halfSize, centroid.z + halfSize);
    // Right face
    glVertex3f(centroid.x + halfSize, centroid.y - halfSize, centroid.z - halfSize);
    glVertex3f(centroid.x + halfSize, centroid.y + halfSize, centroid.z - halfSize);
    glVertex3f(centroid.x + halfSize, centroid.y + halfSize, centroid.z + halfSize);
    glVertex3f(centroid.x + halfSize, centroid.y - halfSize, centroid.z + halfSize);
    // Left face
    glVertex3f(centroid.x - halfSize, centroid.y - halfSize, centroid.z - halfSize);
    glVertex3f(centroid.x - halfSize, centroid.y - halfSize, centroid.z + halfSize);
    glVertex3f(centroid.x - halfSize, centroid.y + halfSize, centroid.z + halfSize);
    glVertex3f(centroid.x - halfSize, centroid.y + halfSize, centroid.z - halfSize);
    glEnd(); // Done drawing the quad
}

void display(const std::vector<Polygon>& polygons, const std::vector<int>& path) {
    glLoadIdentity(); // Reset the model-view matrix
    gluLookAt(15.0, 15.0, 40.0,  // Camera position
              5.0, 5.0, 0.0,    // Look at point
              0.0, 1.0, 0.0);   // Up vector

    // Draw the grid
    for (const Polygon& polygon : polygons) {
        glColor3f(0.5f, 0.5f, 0.5f); // Set the color to gray
        drawCube(polygon.centroid, 0.9f);
    }

    // Draw the path
    glColor3f(1.0f, 0.0f, 0.0f); // Set the color to red
    for (int index : path) {
        drawCube(polygons[index].centroid, 1.0f);
    }
}

