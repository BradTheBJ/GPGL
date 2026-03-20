#pragma once
#include <glad/glad.h>
#include <string_view>
#include <GLFW/glfw3.h>
#include "color.h"

using uint = unsigned int;

namespace s3dl
{
    class Window
    {
    public:
        Window(const uint& width, const uint& height, std::string_view title);
        ~Window();

        bool shouldWindowClose() const;

        void processEvents();

        void setClearColor(const Color& color);

        void clear();

        void display();

    private:
        uint m_width;
        uint m_height;
        std::string_view m_title;
        GLFWwindow* m_window;
        Color m_color{255,255,255,255}; // Defult color to white
    };
}
