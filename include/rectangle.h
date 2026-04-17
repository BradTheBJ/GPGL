#pragma once
#include "shape.h"
#include <array>

namespace gpgl {
class Rectangle : public Shape {
  public:
    Rectangle(const float& width, const float& height, Window& window);

    void draw() override;

  private:
    std::array<GLfloat, 12> m_vertices;
    std::array<GLuint, 6> m_indices;
    float m_width = 0.0f, m_height = 0.0f;

    void updateVertices();
};
} // namespace gpgl
