#include "input.h"

namespace gpgl {

    bool InputHandler::isKeyDown(Keys key) const {
        return glfwGetKey(m_window.getWindow(), static_cast<int>(key)) == GLFW_PRESS;
    }

    bool InputHandler::isKeyUp(Keys key) const {
        return glfwGetKey(m_window.getWindow(), static_cast<int>(key)) == GLFW_RELEASE;
    }

    bool InputHandler::isKeyPressed(Keys key) const {
        return glfwGetKey(m_window.getWindow(), static_cast<int>(key)) == GLFW_ PRESS;
    }

    bool InputHandler::isKeyReleased(Keys key) const {
        return glfwGetKey(m_window.getWindow(), static_cast<int>(key)) == GLFW_RELEASE;
    }
} // namespace gpgl