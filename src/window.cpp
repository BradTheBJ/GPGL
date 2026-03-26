#include "window.h"

// glad/glad.h and GLFW/glfw3.h are already pulled in (correct order) via window.h
#include <cstdlib>
#include <iostream>

namespace gpgl {
Window::Window(const uint& width, const uint& height, std::string_view title) {
    glfwInit(); // Start up the GLFW library

    m_width = width;
    m_height = height;
    m_title = title;

    // Request an OpenGL 3.3 Core Profile context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window =
        glfwCreateWindow(m_width, m_height, m_title.data(), nullptr, nullptr);

    if (!m_window) {
        throw new std::runtime_error("Failed to create GLFW window.");
        glfwTerminate();
    }

    // Make this window's OpenGL context current on the calling thread
    glfwMakeContextCurrent(m_window);

    // Load all OpenGL function pointers via GLAD; must happen after context creation
    if (!gladLoadGL()) {
        throw new std::runtime_error("Failed to initialize GLAD.");
    }

    // Store a pointer to this Window so the static callback can reach it
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

    // Query actual framebuffer size (may differ from window size on HiDPI displays)
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(m_window, &fbWidth, &fbHeight);
    m_width = static_cast<uint>(fbWidth);
    m_height = static_cast<uint>(fbHeight);
    // Set the OpenGL viewport to cover the full framebuffer
    glViewport(0, 0, fbWidth, fbHeight);
}

Window::~Window() {
    glfwDestroyWindow(m_window);
}

bool Window::shouldWindowClose() const {
    return glfwWindowShouldClose(m_window);
}

void Window::processEvents() { glfwPollEvents(); }

void Window::setClearColor(const Color& color) {
    m_color = color;
    // glClearColor sets the RGBA value used when glClear wipes the color buffer
    glClearColor(color.r, color.g, color.b, color.a);
}

void Window::clear() {
    // Wipe the color buffer with the value set by glClearColor
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::display() {
    // Swap the back buffer (rendered frame) with the front buffer (display)
    glfwSwapBuffers(m_window);
}

uint Window::getWidth() const { return m_width; }

uint Window::getHeight() const { return m_height; }

void Window::framebufferSizeCallback(GLFWwindow* window, int width,
                                     int height) {
    // Retrieve the Window instance stored in the GLFW user pointer
    auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (self) {
        self->m_width = static_cast<uint>(width);
        self->m_height = static_cast<uint>(height);
    }
    // Resize the OpenGL viewport to match the new framebuffer dimensions
    glViewport(0, 0, width, height);
}
} // namespace gpgl
