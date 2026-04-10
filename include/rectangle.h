#pragma once
#include "shader.h"
#include "window.h"
#include <array>
#include <glad/glad.h>

namespace gpgl {
class Rectangle {
  public:
    Rectangle(const float& width, const float& height, Window& window);
    ~Rectangle();

    void draw();

    void setPosition(const float& x, const float& y);
    const float& getPositionX() const;
    const float& getPositionY() const;

    void rotate(const float& angle);

  private:
    Shader m_shader{"../shaders/fragShader.glsl"};
    GLuint m_VBO = 0; // Vertex Buffer Object — holds vertex data on the GPU
    GLuint m_VAO = 0; // Vertex Array Object — records buffer/attribute bindings
    GLuint m_EBO =
        0; // Element Buffer Object — holds index data for indexed drawing
    std::array<GLfloat, 12> m_vertices; // 4 vertices × 3 floats (x, y, z)
    std::array<GLuint, 6> m_indices;    // 2 triangles × 3 indices
    float m_x = 0.0f, m_y = 0.0f;
    float m_rotation = 0.0f;
    float m_width = 0.0f, m_height = 0.0f;
    Window* m_pWindow = nullptr;

    void updateVertices(); // Upload local pixel positions to GPU
};
} // namespace gpgl
