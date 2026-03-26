#include "window.h"

#include <iostream>
#include <cstdlib>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace gpgl
{
    Window::Window(const uint& width, const uint& height, std::string_view title)
    {
        glfwInit();

        m_width  = width;
        m_height = height;
        m_title  = title;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(m_width, m_height, m_title.data(), nullptr, nullptr);

        if (!m_window)
        {
            std::cerr << "Failed to create GLFW window\n";
            glfwTerminate();
            std::exit(1);
        }

        glfwMakeContextCurrent(m_window);

        if (!gladLoadGL())
        {
            std::cerr << "Failed to initialize GLAD\n";
            std::exit(1);
        }

        glfwSetWindowUserPointer(m_window, this);
        glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

        int fbWidth, fbHeight;
        glfwGetFramebufferSize(m_window, &fbWidth, &fbHeight);
        m_width  = static_cast<uint>(fbWidth);
        m_height = static_cast<uint>(fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    bool Window::shouldWindowClose() const
    {
        return glfwWindowShouldClose(m_window);
    }

    void Window::processEvents()
    {
        glfwPollEvents();
    }

    void Window::setClearColor(const Color& color)
    {
        m_color = color;
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void Window::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Window::display()
    {
        glfwSwapBuffers(m_window);
    }

    uint Window::getWidth() const
    {
        return m_width;
    }

    uint Window::getHeight() const
    {
        return m_height;
    }

    void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (self)
        {
            self->m_width  = static_cast<uint>(width);
            self->m_height = static_cast<uint>(height);
        }
        glViewport(0, 0, width, height);
    }
} // namespace gpgl
