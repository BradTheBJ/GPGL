#include "shape.h"
#include "texture.h"
#include <numbers>

namespace gpgl {

Shape::Shape(Window& window) : m_window(window), m_shader() {}

Shape::~Shape() {
    if (m_VAO)
        glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO)
        glDeleteBuffers(1, &m_VBO);
    if (m_EBO)
        glDeleteBuffers(1, &m_EBO);
}

void Shape::setPosition(const float& x, const float& y) {
    m_x = x;
    m_y = y;
}

const float& Shape::getPositionX() const { return m_x; }

const float& Shape::getPositionY() const { return m_y; }

void Shape::rotate(const float& angle) { m_rotation += angle; }

void Shape::setTexture(Texture& texture) { m_texture = &texture; }

void Shape::prepareDraw() {
    m_shader.use();
    m_shader.setFloat("u_x", m_x);
    m_shader.setFloat("u_y", m_y);
    m_shader.setFloat("u_rotation",
                      m_rotation * (std::numbers::pi_v<float> / 180.0f));
    m_shader.setFloat("u_windowWidth", static_cast<float>(m_window.getWidth()));
    m_shader.setFloat("u_windowHeight",
                      static_cast<float>(m_window.getHeight()));

    if (m_texture) {
        m_texture->bind(0);
        m_shader.setInt("u_useTexture", 1);
        m_shader.setInt("ourTexture", 0);
    } else {
        m_shader.setInt("u_useTexture", 0);
    }
}

} // namespace gpgl
