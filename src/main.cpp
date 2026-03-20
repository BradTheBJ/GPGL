#include <iostream>
#include "window.h"
#include "triangle.h"

int main()
{
    std::cout << "Hello World!" << std::endl;
    s3dl::Window window(800, 600, "Hello OpenGL!");
    s3dl::Color color(20, 20, 100, 255);
    s3dl::Triangle trig;
    window.setClearColor(color);
    while (!window.shouldWindowClose())
    {
        window.processEvents();
        window.clear();
        trig.draw();
        window.display();
    }
}