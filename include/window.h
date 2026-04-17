#pragma once
#include "color.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string_view>

using uint = unsigned int;

namespace gpgl {
class Window {
  public:
    Window(const uint& width, const uint& height, std::string_view title);
    ~Window();

    bool shouldWindowClose() const;

    void processEvents();

    void setClearColor(const Color& color);

    void clear();

    void display();

    uint getWidth() const;
    uint getHeight() const;
    GLFWwindow* getNativeWindow() const;

  private:
    uint m_width;
    uint m_height;
    std::string_view m_title;
    GLFWwindow* m_window;
    Color m_color{255, 255, 255, 255}; // Default color to white

    static void framebufferSizeCallback(GLFWwindow* window, int width,
                                        int height);
};
} // namespace gpgl
