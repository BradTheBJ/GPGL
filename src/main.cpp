#include "window.h"
#include "rectangle.h"
#include "input.h"

int main() {
    gpgl::Window window(800, 600, "GPGL Example");
    window.setClearColor(gpgl::Color(100,100,100,255));
    gpgl::InputHandler input(window);
    gpgl::Rectangle rect(100,100,window);
    rect.setPosition(window.getWidth() / 2,window.getHeight() / 2);

    while (!window.shouldWindowClose()) {
        window.processEvents();
        window.clear();
        if (input.isKeyDown(gpgl::Keys::A)) {
            rect.setPosition(rect.getPositionX() - 5 , rect.getPositionY());
        }
        if (input.isKeyDown(gpgl::Keys::D)) {
            rect.setPosition(rect.getPositionX() + 5 , rect.getPositionY());
        }
        if (input.isKeyDown(gpgl::Keys::W)) {
            rect.setPosition(rect.getPositionX() , rect.getPositionY() - 5);
        }
        if (input.isKeyDown(gpgl::Keys::S)) {
            rect.setPosition(rect.getPositionX(), rect.getPositionY() + 5);
        }
        rect.draw();
        window.display();
    }
}