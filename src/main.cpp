
#include "texture.h"
#include "triangle.h"
#include "rectangle.h"
#include "window.h"


int main() {
    // Example script of GPGL
    gpgl::Window window(800, 600, "GPGL Example");
    gpgl::Rectangle rectangle(200.0f, 200.0f, window);
    rectangle.setPosition(400.0f, 300.0f);

    gpgl::Texture tex("images/dog.jpg");
    rectangle.setTexture(tex);

    while (!window.shouldWindowClose()) {
        window.processEvents();
        window.clear();
        
        rectangle.draw();
        
        window.display();
    }
}