#include "rectangle.h"

namespace gpgl {
Rectangle::Rectangle(const float& width, const float& height, Window& window)
    : Shape(window), m_width(width), m_height(height) {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    // Local coordinates (pixels) relative to center, with UV coords
    m_vertices = {
        // Position          // UVs
        -m_width / 2.0f, -m_height / 2.0f, 0.0f, 0.0f, 1.0f, // bottom left
         m_width / 2.0f, -m_height / 2.0f, 0.0f, 1.0f, 1.0f, // bottom right
         m_width / 2.0f,  m_height / 2.0f, 0.0f, 1.0f, 0.0f, // top right
        -m_width / 2.0f,  m_height / 2.0f, 0.0f, 0.0f, 0.0f  // top left
    };

    m_indices = {0, 1, 2, 0, 2, 3};

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Rectangle::draw() {
    prepareDraw(); // handles shader, uniforms, and texture binding
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Rectangle::updateVertices() {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(float), m_vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace gpgl
