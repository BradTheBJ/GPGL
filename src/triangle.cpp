#include "triangle.h"

#include <stdexcept>

namespace gpgl {
Triangle::Triangle(const float& base, const float& height, Window& window)
    : m_pWindow(&window), m_base(base), m_height(height) {
    if (base < 0.0f)
        throw std::invalid_argument("base must be greater than 0.0");
    if (height < 0.0f)
        throw std::invalid_argument("height must be greater than 0.0");

    calculateShaders();
}

Triangle::~Triangle() {
    // Release GPU resources in reverse order of creation
    glDeleteVertexArrays(1, &m_VAO); // Unbind and delete the vertex array object
    glDeleteBuffers(1, &m_VBO);      // Free the vertex buffer on the GPU
    glDeleteProgram(m_shaderProgram); // Destroy the linked shader program
}

void Triangle::draw() {
    // Activate the shader program for this triangle
    glUseProgram(m_shaderProgram); // Sets the active shader program for subsequent draw calls
    glBindVertexArray(m_VAO);      // Restore the vertex layout and buffer bindings
    glDrawArrays(GL_TRIANGLES, 0, 3); // Draw 3 vertices as a single triangle (no index buffer)
}

void Triangle::setPosition(const float& x, const float& y) {
    m_x = x;
    m_y = y;
    updateVertices();
}

const float& Triangle::getPositionX() const { return m_x; }

const float& Triangle::getPositionY() const { return m_y; }

void Triangle::setFrag(const std::filesystem::path& path) {
    m_fragmentShaderSource = Shader(path);
    calculateShaders();
}

void Triangle::setVertex(const std::filesystem::path& path) {
    m_vertexShaderSource = Shader(path);
    calculateShaders();
}

void Triangle::updateVertices() {
    if (!m_pWindow)
        return;

    float width = static_cast<float>(m_pWindow->getWidth());
    float height = static_cast<float>(m_pWindow->getHeight());

    // Convert pixel coordinates (m_x, m_y) to Normalized Device Coordinates
    // (NDC). NDC range is [-1, 1] on both axes; (0,0) is top-left in pixel space.
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
    // using glBufferSubData instead of glBufferData to avoid a full reallocation
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(float),
                    m_vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind to prevent accidental modification
}

void Triangle::calculateShaders() {
    // Cleanup existing resources if any
    if (m_shaderProgram)
        glDeleteProgram(m_shaderProgram);
    if (m_VAO)
        glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO)
        glDeleteBuffers(1, &m_VBO);

    // Shader setup
    // glCreateShader allocates a shader object on the GPU and returns its handle
    m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vs = m_vertexShaderSource.c_str();
    // Upload the GLSL source code; the count (1) is the number of source strings
    glShaderSource(m_vertexShader, 1, &vs, nullptr);
    glCompileShader(m_vertexShader); // Compile to GPU bytecode

    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fs = m_fragmentShaderSource.c_str();
    glShaderSource(m_fragmentShader, 1, &fs, nullptr);
    glCompileShader(m_fragmentShader);

    // Create a shader program and attach the compiled shaders
    // glCreateProgram returns a handle to a new program object that shaders link into
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, m_vertexShader);
    glAttachShader(m_shaderProgram, m_fragmentShader);
    glLinkProgram(m_shaderProgram); // Link the stages into a complete pipeline

    // Cleanup shader objects; they are now baked into the program
    glDeleteShader(m_vertexShader);
    glDeleteShader(m_fragmentShader);

    // VBO/VAO setup
    // glGenVertexArrays creates a VAO that records all vertex attribute state
    glGenVertexArrays(1, &m_VAO);
    // glGenBuffers creates a VBO to hold vertex data on the GPU
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO); // Begin recording vertex state into the VAO
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO); // Bind the VBO as the active array buffer

    // Allocate and upload vertex data; GL_DYNAMIC_DRAW hints that data will be
    // updated frequently, allowing the driver to place it in faster memory
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float),
                 m_vertices.data(), GL_DYNAMIC_DRAW);

    // Describe attribute 0: 3 floats per vertex, tightly packed, starting at offset 0.
    // This tells the vertex shader how to read each vertex from the VBO.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0); // Enable attribute slot 0 for the VAO

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO (VAO retains the reference)
    glBindVertexArray(0);             // Unbind VAO to prevent further accidental state changes

    // Initial calculation based on window dimensions
    updateVertices();
}
} // namespace gpgl
