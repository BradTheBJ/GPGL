#include "rectangle.h"
#include "triangle.h"
#include "window.h"
#include <iostream>

int main() {
    // Example script of GPGL
    std::cout << "Hello World!" << std::endl;
    gpgl::Window window(1920, 1080, "Hello OpenGL!");
    gpgl::Color color(20, 20, 100, 255);
    gpgl::Triangle trig(100, 100, window);
    trig.setPosition(window.getWidth() / 2, window.getHeight() / 2);
    window.setClearColor(color);
    while (!window.shouldWindowClose()) {
        window.processEvents();
        window.clear();
        trig.rotate(1);
        trig.draw();
        window.display();
    }
}