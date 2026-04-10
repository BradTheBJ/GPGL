#include "rectangle.h"
#include "triangle.h"
#include "window.h"
#include <iostream>

int main() {
    std::cout << "Hello World!" << std::endl;
    gpgl::Window window(1920, 1080, "Hello OpenGL!");
    gpgl::Color color(20, 20, 100, 255);
    gpgl::Triangle trig(100, 100, window);
    trig.setPosition(960, 540); // Center of 1920x1080
    
    window.setClearColor(color);
    while (!window.shouldWindowClose()) {
        window.processEvents();
        window.clear();
        
        trig.rotate(1.0f); // Rotate by 1 degree per frame
        trig.draw();
        
        window.display();
    }
}