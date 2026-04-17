#include "triangle.h"
#include <numbers>

namespace gpgl {
Triangle::Triangle(const float& base, const float& height, Window& window)
    : m_pWindow(&window), m_base(base), m_height(height) {
    if (base < 0.0f)
        throw std::invalid_argument("base must be greater than 0.0");
    if (height < 0.0f)
        throw std::invalid_argument("height must be greater than 0.0");

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    
    // Local coordinates (pixels) relative to center
    m_vertices = {
        0.0f,  m_height / 2.0f, 0.0f, // top
       -m_base / 2.0f, -m_height / 2.0f, 0.0f, // bottom left
        m_base / 2.0f, -m_height / 2.0f, 0.0f  // bottom right
    };

    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Triangle::~Triangle() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void Triangle::draw() {
    m_shader.use();

    // Set transformation uniforms
    m_shader.setFloat("u_x", m_x);
    m_shader.setFloat("u_y", m_y);
    m_shader.setFloat("u_rotation", m_rotation * (std::numbers::pi_v<float> / 180.0f));
    if (m_pWindow) {
        m_shader.setFloat("u_windowWidth", static_cast<float>(m_pWindow->getWidth()));
        m_shader.setFloat("u_windowHeight", static_cast<float>(m_pWindow->getHeight()));
    }

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Triangle::setPosition(const float& x, const float& y) {
    m_x = x;
    m_y = y;
}

const float& Triangle::getPositionX() const { return m_x; }

const float& Triangle::getPositionY() const { return m_y; }

void Triangle::updateVertices() {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(float), m_vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Triangle::rotate(const float& degrees) {
    m_rotation += degrees;
}

} // namespace gpgl
