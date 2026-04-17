#include "rectangle.h"
#include "triangle.h"
#include "window.h"
#include "input.h"
#include <iostream>

int main() {
    // Example script of GPGL
    gpgl::Window window(800, 600, "GPGL Example");
    gpgl::Rectangle rectangle(200.0f, 200.0f, window);
    rectangle.setPosition(400.0f, 300.0f); // Center of the window
    window.setClearColor(gpgl::Color(255, 255, 255, 255)); // White background
    gpgl::InputHandler inputHandler(window);

    while (!window.shouldWindowClose()) {
        window.processEvents();
        window.clear();

        float speed = 2.0f;

        if (inputHandler.isKeyDown(gpgl::Keys::A)) {
            rectangle.setPosition(rectangle.getPositionX() - speed, rectangle.getPositionY()); // Move left
        }
        if (inputHandler.isKeyDown(gpgl::Keys::D)) {
            rectangle.setPosition(rectangle.getPositionX() + speed, rectangle.getPositionY()); // Move right
        }
        if (inputHandler.isKeyDown(gpgl::Keys::W)) {
            rectangle.setPosition(rectangle.getPositionX(), rectangle.getPositionY() - speed); // Move up
        }
        if (inputHandler.isKeyDown(gpgl::Keys::S)) {        
            rectangle.setPosition(rectangle.getPositionX(), rectangle.getPositionY() + speed); // Move down
        }

        rectangle.draw();
        window.display();
    }
}