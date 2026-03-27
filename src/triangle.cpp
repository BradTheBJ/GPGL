#include "triangle.h"

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
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    updateVertices();
}

Triangle::~Triangle() {
    // Release GPU resources in reverse order of creation
    glDeleteVertexArrays(1,
                         &m_VAO); // Unbind and delete the vertex array object
    // Free the vertex buffer on the GPU
}

void Triangle::draw() {
    // Activate the shader program for this triangle
    m_shader.use();
    glBindVertexArray(m_VAO); // Restore the vertex layout and buffer bindings
    glDrawArrays(GL_TRIANGLES, 0,
                 3); // Draw 3 vertices as a single triangle (no index buffer)
}

void Triangle::setPosition(const float& x, const float& y) {
    m_x = x;
    m_y = y;
    updateVertices();
}

const float& Triangle::getPositionX() const { return m_x; }

const float& Triangle::getPositionY() const { return m_y; }

void Triangle::updateVertices() {
    if (!m_pWindow)
        return;

    float width = static_cast<float>(m_pWindow->getWidth());
    float height = static_cast<float>(m_pWindow->getHeight());

    // Convert pixel coordinates (m_x, m_y) to Normalized Device Coordinates
    // (NDC). NDC range is [-1, 1] on both axes; (0,0) is top-left in pixel
    // space.
    float ndcX = (m_x / (width / 2.0f)) - 1.0f;
    float ndcY = 1.0f - (m_y / (height / 2.0f));

    float ndcBase = m_base / (width / 2.0f);
    float ndcHeight = m_height / (height / 2.0f);

    m_vertices = {
        ndcX,
        ndcY + ndcHeight / 2.0f,
        0.0f, // top
        ndcX - ndcBase / 2.0f,
        ndcY - ndcHeight / 2.0f,
        0.0f, // bottom left
        ndcX + ndcBase / 2.0f,
        ndcY - ndcHeight / 2.0f,
        0.0f // bottom right
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

void Triangle::setFrag(const std::filesystem::path& path) {
    m_shader = Shader("../shaders/vertShader.glsl", path);
}

void Triangle::setVertex(const std::filesystem::path& path) {
    m_shader = Shader(path, "../shaders/fragShader.glsl");
}
} // namespace gpgl
