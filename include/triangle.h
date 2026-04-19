#pragma once
#include "shape.h"
#include <array>

namespace gpgl {
class Triangle : public Shape {
  public:
    Triangle(const float& base, const float& height, Window& window);

    void draw() override;

  private:
    float m_base = 0.0f, m_height = 0.0f;
    std::array<GLfloat, 15> m_vertices; // 3 vertices * 5 floats (x,y,z,u,v)

    void updateVertices();
};
} // namespace gpgl
