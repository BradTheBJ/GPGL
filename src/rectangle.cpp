#include "rectangle.h"

namespace gpgl
{
    Rectangle::Rectangle()
    : m_vertices{
        -0.5f, -0.5f, 0.0f,  // 0 bottom left
         0.5f, -0.5f, 0.0f,  // 1 bottom right
         0.5f,  0.5f, 0.0f,  // 2 top right
        -0.5f,  0.5f, 0.0f   // 3 top left
    },
    m_indices{
        0, 1, 2,
        0, 2, 3
    }
    {
        m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* vs = m_vertexShaderSource.c_str();
        glShaderSource(m_vertexShader, 1, &vs, nullptr);
        glCompileShader(m_vertexShader);

        m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fs = m_fragmentShaderSource.c_str();
        glShaderSource(m_fragmentShader, 1, &fs, nullptr);
        glCompileShader(m_fragmentShader);

        m_shaderProgram = glCreateProgram();
        glAttachShader(m_shaderProgram, m_vertexShader);
        glAttachShader(m_shaderProgram, m_fragmentShader);
        glLinkProgram(m_shaderProgram);

        glDeleteShader(m_vertexShader);
        glDeleteShader(m_fragmentShader);

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER,
                     m_vertices.size() * sizeof(float),
                     m_vertices.data(),
                     GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     m_indices.size() * sizeof(GLuint),
                     m_indices.data(),
                     GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }

    Rectangle::~Rectangle()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
        glDeleteProgram(m_shaderProgram);
    }

    void Rectangle::draw()
    {
        glUseProgram(m_shaderProgram);
        glBindVertexArray(m_VAO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}
