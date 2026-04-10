#pragma once
#include "shader.h"
#include "window.h"
#include <array>
#include <glad/glad.h>

namespace gpgl {
class Triangle {
  public:
    Triangle(const float& base, const float& height, Window& window);
    ~Triangle();

    void draw();

    void setPosition(const float& x, const float& y);
    const float& getPositionX() const;
    const float& getPositionY() const;

    void rotate(const float& angle);

  private:
    Shader m_shader{"../shaders/fragShader.glsl"};
    GLuint m_VBO = 0; // Vertex Buffer Object — holds vertex data on the GPU
    GLuint m_VAO = 0; // Vertex Array Object — records buffer/attribute bindings
    float m_base = 0.0f, m_height = 0.0f;
    GLfloat m_x = 0.0f, m_y = 0.0f;
    float m_rotation = 0.0f;
    std::array<GLfloat, 9> m_vertices; // 3 vertices × 3 floats (x, y, z)
    Window* m_pWindow = nullptr;

    void updateVertices(); // Upload local pixel positions to GPU
};
} // namespace gpgl
