#pragma once
#include "window.h"
namespace gpgl {
    class InputHandler {
        public:
            InputHandler(Window& window) : m_window(window) {}
        private:
            Window& m_window;
    };
} // Namespace gpgl