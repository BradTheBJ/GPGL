#include <iostream>
#include "window.h"
#include "triangle.h"
#include "rectangle.h"

int main()
{
    std::cout << "Hello World!" << std::endl;
    gpgl::Window window(800, 600, "Hello OpenGL!");
    gpgl::Color color(20, 20, 100, 255);
    gpgl::Rectangle rect(100,100,window);
    window.setClearColor(color);
    while (!window.shouldWindowClose())
    {
        window.processEvents();
        window.clear();
        rect.draw();
        window.display();
    }
}