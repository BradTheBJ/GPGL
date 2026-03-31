#include "rectangle.h"

namespace gpgl {
Rectangle::Rectangle(const float& width, const float& height, Window& window)
        // Init the vertices based on the user input
        : m_vertices{
            -width / (window.getWidth()  / 2.0f), -height / (window.getHeight() / 2.0f), 0.0f, // bottom left
             width / (window.getWidth()  / 2.0f), -height / (window.getHeight() / 2.0f), 0.0f, // bottom right
             width / (window.getWidth()  / 2.0f),  height / (window.getHeight() / 2.0f), 0.0f, // top right
            -width / (window.getWidth()  / 2.0f),  height / (window.getHeight() / 2.0f), 0.0f  // top left
          },
        // Init indices to form the two triangles covering the rectangle face
          m_indices{
            0, 1, 2, // first triangle  (bottom-left → bottom-right → top-right)
            0, 2, 3  // second triangle (bottom-left → top-right   → top-left)
          },
          m_width(width), m_height(height), m_pWindow(&window)
    {
    // VBO/VAO/EBO setup
    // glGenVertexArrays creates a VAO that records all vertex attribute state
    glGenVertexArrays(1, &m_VAO);
    // glGenBuffers creates buffer objects: VBO for vertices, EBO for indices
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO); // Begin recording vertex state into the VAO

    // Upload vertex positions to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    // GL_DYNAMIC_DRAW hints frequent updates; driver may place in faster memory
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float),
                 m_vertices.data(), GL_DYNAMIC_DRAW);

    // Upload index data so OpenGL knows how to assemble triangles from vertices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint),
                 m_indices.data(), GL_DYNAMIC_DRAW);

    // Describe attribute 0: 3 floats per vertex, tightly packed, starting at
    // offset 0. This tells the vertex shader how to read each vertex from the
    // VBO.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0); // Enable attribute slot 0 for the VAO

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO (VAO retains the reference)
    glBindVertexArray(0); // Unbind VAO to prevent accidental state changes

    // Initial calculation based on window dimensions
    updateVertices();
}

Rectangle::~Rectangle() {
    // Release GPU resources in reverse order of creation
    glDeleteVertexArrays(1,
                         &m_VAO); // Unbind and delete the vertex array object
    glDeleteBuffers(1, &m_VBO);   // Free the vertex buffer on the GPU
    glDeleteBuffers(1, &m_EBO);   // Free the index buffer on the GPU
}

void Rectangle::draw() {
    // Activate the shader program for this rectangle
    m_shader.use();
    glBindVertexArray(m_VAO); // Restore the vertex layout and buffer bindings
    // Draw 6 indices (2 triangles) as unsigned ints; no offset into the EBO
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Rectangle::setPosition(const float& x, const float& y) {
    m_x = x;
    m_y = y;
    updateVertices();
}

const float& Rectangle::getPositionX() const { return m_x; }

const float& Rectangle::getPositionY() const { return m_y; }

void Rectangle::updateVertices() {
    if (!m_pWindow)
        return;

    float width = static_cast<float>(m_pWindow->getWidth());
    float height = static_cast<float>(m_pWindow->getHeight());

    // Convert pixel coordinates (m_x, m_y) to Normalized Device Coordinates
    // (NDC). NDC range is [-1, 1] on both axes; (0,0) is top-left in pixel
    // space.
    float ndcX = (m_x / (width / 2.0f)) - 1.0f;
    float ndcY = 1.0f - (m_y / (height / 2.0f));

    float ndcHalfWidth = m_width / width;
    float ndcHalfHeight = m_height / height;

    // Recalculate vertices centered at (ndcX, ndcY)
    m_vertices = {
        ndcX - ndcHalfWidth, ndcY - ndcHalfHeight, 0.0f, // bottom left
        ndcX + ndcHalfWidth, ndcY - ndcHalfHeight, 0.0f, // bottom right
        ndcX + ndcHalfWidth, ndcY + ndcHalfHeight, 0.0f, // top right
        ndcX - ndcHalfWidth, ndcY + ndcHalfHeight, 0.0f  // top left
    };

    // Push updated vertex data to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO); // Target the VBO for writing
    // Overwrite a sub-range of the buffer (offset 0, full vertex array size)
    // using glBufferSubData instead of glBufferData to avoid a full
    // reallocation
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(float),
                    m_vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER,
                 0); // Unbind to prevent accidental modification
}

} // namespace gpgl
