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
    // Shader setup
    int success;
    char infoLog[512];  // character buffer

    m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vs = m_vertexShaderSource.c_str();
    // Upload the GLSL source; the count (1) is the number of source strings
    glShaderSource(m_vertexShader, 1, &vs, nullptr);
    glCompileShader(m_vertexShader);
    glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) { // Check if the vertex shader compiled
        glGetShaderInfoLog(m_vertexShader, 512, NULL, infoLog);
        throw std::runtime_error(std::string("VERTEX SHADER COMPILATION FAILED: ") + infoLog);
    }

    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fs = m_fragmentShaderSource.c_str();
    // Upload fragment shader source and compile
    glShaderSource(m_fragmentShader, 1, &fs, nullptr);
    glCompileShader(m_fragmentShader);
    glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) { // Check if the vertex shader compiled
        glGetShaderInfoLog(m_fragmentShader, 512, NULL, infoLog);
        throw std::runtime_error(std::string("FRAGMENT SHADER COMPILATION FAILED: ") + infoLog);
    }

    // Create a shader program and attach the compiled shaders into the shader program
    // glCreateProgram returns a handle to a new program object that shaders link into
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, m_vertexShader);
    glAttachShader(m_shaderProgram, m_fragmentShader);
    glLinkProgram(m_shaderProgram); // Link the stages into a complete pipeline

    // Cleanup shaders as they are already baked into the shader program
    glDeleteShader(m_vertexShader);
    glDeleteShader(m_fragmentShader);

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

    // Describe attribute 0: 3 floats per vertex, tightly packed, starting at offset 0.
    // This tells the vertex shader how to read each vertex from the VBO.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0); // Enable attribute slot 0 for the VAO

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO (VAO retains the reference)
    glBindVertexArray(0);             // Unbind VAO to prevent accidental state changes

    // Initial calculation based on window dimensions
    updateVertices();
}

Rectangle::~Rectangle() {
    // Release GPU resources in reverse order of creation
    glDeleteVertexArrays(1, &m_VAO); // Unbind and delete the vertex array object
    glDeleteBuffers(1, &m_VBO);      // Free the vertex buffer on the GPU
    glDeleteBuffers(1, &m_EBO);      // Free the index buffer on the GPU
    glDeleteProgram(m_shaderProgram); // Destroy the linked shader program
}

void Rectangle::draw() {
    // Activate the shader program for this rectangle
    glUseProgram(m_shaderProgram); // Sets the active shader program for subsequent draw calls
    glBindVertexArray(m_VAO);      // Restore the vertex layout and buffer bindings
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

void Rectangle::setFrag(const std::filesystem::path& path) {
    m_fragmentShaderSource = Shader(path);
    calculateShaders();
}

void Rectangle::setVertex(const std::filesystem::path& path) {
    m_vertexShaderSource = Shader(path);
    calculateShaders();
}

void Rectangle::updateVertices() {
    if (!m_pWindow)
        return;

    float width = static_cast<float>(m_pWindow->getWidth());
    float height = static_cast<float>(m_pWindow->getHeight());

    // Convert pixel coordinates (m_x, m_y) to Normalized Device Coordinates
    // (NDC). NDC range is [-1, 1] on both axes; (0,0) is top-left in pixel space.
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
    // using glBufferSubData instead of glBufferData to avoid a full reallocation
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(float),
                    m_vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind to prevent accidental modification
}

void Rectangle::calculateShaders() {
    // Cleanup existing resources if any
    if (m_shaderProgram)
        glDeleteProgram(m_shaderProgram);
    if (m_VAO)
        glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO)
        glDeleteBuffers(1, &m_VBO);
    if (m_EBO)
        glDeleteBuffers(1, &m_EBO);

    // Shader setup
    m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vs = m_vertexShaderSource.c_str();
    glShaderSource(m_vertexShader, 1, &vs, nullptr);
    glCompileShader(m_vertexShader);

    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fs = m_fragmentShaderSource.c_str();
    glShaderSource(m_fragmentShader, 1, &fs, nullptr);
    glCompileShader(m_fragmentShader);

    // Create a shader program and attach the compiled shaders
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, m_vertexShader);
    glAttachShader(m_shaderProgram, m_fragmentShader);
    glLinkProgram(m_shaderProgram);

    // Cleanup shaders as they are already baked into the program
    glDeleteShader(m_vertexShader);
    glDeleteShader(m_fragmentShader);

    // VBO/VAO/EBO setup
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float),
                 m_vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint),
                 m_indices.data(), GL_DYNAMIC_DRAW);

    // Describe attribute 0: 3 floats per vertex, tightly packed, starting at offset 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Initial calculation based on window dimensions
    updateVertices();
}
} // namespace gpgl
