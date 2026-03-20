#include "window.h"

#include <iostream>
#include <ostream>
#include <glad/glad.h>

namespace s3dl
{
    Window::Window(const uint& width, const uint& height, std::string_view title)
    {
        glfwInit();
        m_width = width;
        m_height = height;
        m_title = title;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        m_window = glfwCreateWindow(m_width, m_height, m_title.data(), nullptr, nullptr);

        if (!m_window)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            std::exit(1);
        }

        glfwMakeContextCurrent(m_window);

        gladLoadGL();

        glViewport(0, 0, m_width, m_height);
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
}
