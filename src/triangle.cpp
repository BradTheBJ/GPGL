#include "triangle.h"
#include <stdexcept>

namespace gpgl {
Triangle::Triangle(const float& base, const float& height, Window& window)
    : Shape(window), m_base(base), m_height(height) {
    if (base < 0.0f)
        throw std::invalid_argument("base must be greater than 0.0");
    if (height < 0.0f)
        throw std::invalid_argument("height must be greater than 0.0");

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // Local coordinates (pixels) relative to center, with default UVs
    m_vertices = {
        // Position          // UVs
        0.0f,           m_height / 2.0f,  0.0f, 0.5f, 0.0f, // top
        -m_base / 2.0f, -m_height / 2.0f, 0.0f, 0.0f, 1.0f, // bottom left
        m_base / 2.0f,  -m_height / 2.0f, 0.0f, 1.0f, 1.0f  // bottom right
    };

    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float),
                 m_vertices.data(), GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute (location 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Triangle::draw() {
    prepareDraw(); // sets ID and other uniforms

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Triangle::updateVertices() {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(float),
                    m_vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace gpgl
