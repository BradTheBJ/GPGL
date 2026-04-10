#include "rectangle.h"
#include "triangle.h"
#include "window.h"
#include <iostream>

int main() {
    std::cout << "Hello World!" << std::endl;
    gpgl::Window window(1920, 1080, "Hello OpenGL!");
    gpgl::Color color(20, 20, 100, 255);
    gpgl::Rectangle rect(100, 100, window);
    rect.setPosition(200, 500);
    window.setClearColor(color);
    while (!window.shouldWindowClose()) {
        window.processEvents();
        window.clear();
        rect.rotate(1);
        rect.draw();
        window.display();
    }
}