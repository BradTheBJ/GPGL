#include "rectangle.h"

namespace gpgl {
Rectangle::Rectangle(const float& width, const float& height, Window& window)
    : Shape(window), m_width(width), m_height(height) {
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

void Rectangle::draw() {
    prepareDraw(); // Let the base class set the uniforms (scale, rotation, window coords)
    
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Rectangle::updateVertices() {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(float), m_vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace gpgl
