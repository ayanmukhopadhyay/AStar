#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <GL/glu.h>

// Function prototypes
void initOpenGL();
void drawScene();

int main() {
    // Create an SFML window with a modern OpenGL context
    sf::ContextSettings settings;
    settings.depthBits = 24;  // Request a 24-bit depth buffer
    settings.stencilBits = 0; // Request no stencil buffer
    settings.antialiasingLevel = 0; // Request no anti-aliasing
    settings.majorVersion = 3; // Request OpenGL version 3.3
    settings.minorVersion = 3;

    sf::Window window(sf::VideoMode(800, 600), "OpenGL Test", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    // Check the settings that were actually used
    sf::ContextSettings actual = window.getSettings();
    std::cout << "Requested: OpenGL version = " << settings.majorVersion << "." << settings.minorVersion
            << " ; depth bits = " << settings.depthBits
            << " ; stencil bits = " << settings.stencilBits
            << " ; AA level = " << settings.antialiasingLevel << std::endl;
    std::cout << "Created: OpenGL version = " << actual.majorVersion << "." << actual.minorVersion
            << " ; depth bits = " << actual.depthBits
            << " ; stencil bits = " << actual.stencilBits
            << " ; AA level = " << actual.antialiasingLevel << std::endl;

    // Initialize OpenGL settings
    initOpenGL();

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

        // Render the scene
        drawScene();

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

    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);          // Switch to the projection matrix
    glLoadIdentity();                     // Reset the projection matrix
    gluPerspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f); // Set perspective projection
    glMatrixMode(GL_MODELVIEW);           // Switch back to the model-view matrix
}

void drawScene() {
    glLoadIdentity();                     // Reset the model-view matrix
    glTranslatef(0.0f, 0.0f, -6.0f);      // Move the scene back to be visible

    glBegin(GL_TRIANGLES);                // Start drawing a triangle
    glColor3f(1.0f, 0.0f, 0.0f);          // Red color
    glVertex3f(0.0f, 1.0f, 0.0f);         // Top vertex
    glColor3f(0.0f, 1.0f, 0.0f);          // Green color
    glVertex3f(-1.0f, -1.0f, 0.0f);       // Bottom-left vertex
    glColor3f(0.0f, 0.0f, 1.0f);          // Blue color
    glVertex3f(1.0f, -1.0f, 0.0f);        // Bottom-right vertex
    glEnd();                              // Done drawing the triangle
}

