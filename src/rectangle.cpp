#include "rectangle.h"
#include <numbers>

namespace gpgl {
Rectangle::Rectangle(const float& width, const float& height, Window& window)
    : m_width(width), m_height(height), m_pWindow(&window)
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    // Local coordinates (pixels) relative to center
    m_vertices = {
        -m_width / 2.0f, -m_height / 2.0f, 0.0f, // bottom left
         m_width / 2.0f, -m_height / 2.0f, 0.0f, // bottom right
         m_width / 2.0f,  m_height / 2.0f, 0.0f, // top right
        -m_width / 2.0f,  m_height / 2.0f, 0.0f  // top left
    };

    m_indices = {
        0, 1, 2, 
        0, 2, 3
    };

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Rectangle::~Rectangle() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

void Rectangle::draw() {
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
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Rectangle::setPosition(const float& x, const float& y) {
    m_x = x;
    m_y = y;
}

const float& Rectangle::getPositionX() const { return m_x; }

const float& Rectangle::getPositionY() const { return m_y; }

void Rectangle::rotate(const float& degrees) {
    m_rotation += degrees;
}

void Rectangle::updateVertices() {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(float), m_vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace gpgl
