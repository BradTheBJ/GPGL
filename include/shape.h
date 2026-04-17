#pragma once
#include "shader.h"
#include "window.h"
#include <glad/glad.h>

namespace gpgl {

class Shape {
  public:
    Shape(Window& window);
    virtual ~Shape();

    virtual void draw() = 0;

    virtual void setPosition(const float& x, const float& y);
    virtual const float& getPositionX() const;
    virtual const float& getPositionY() const;

    virtual void rotate(const float& angle);

  protected:
    Window& m_window;
    Shader m_shader{"../shaders/fragShader.glsl"};
    GLuint m_VAO = 0;
    GLuint m_VBO = 0;
    GLuint m_EBO = 0;
    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_rotation = 0.0f;

    void prepareDraw();
};

} // namespace gpgl