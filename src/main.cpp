#include "rectangle.h"
#include "triangle.h"
#include "window.h"
#include <iostream>

int main() {
    // Example script of GPGL
    gpgl::Window window(800, 600, "GPGL Example");
    gpgl::Rectangle rectangle(200.0f, 200.0f, window);
    rectangle.setPosition(400.0f, 300.0f); // Center of the window
    window.setClearColor(gpgl::Color(255, 255, 255, 255)); // White background
    while (!window.shouldWindowClose()) {
        window.processEvents();
        window.clear();

        rectangle.draw();

        window.display();
    }
}