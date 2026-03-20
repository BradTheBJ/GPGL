#include "triangle.h"

#include "GLFW/glfw3.h"

namespace gpgl
{
    Triangle::Triangle()
        : m_vertices{
            0.0f, 0.5f, 0.0f,
             -0.5f, -0.5f, 0.0f,
             0.5f,  -0.5f, 0.0f
        }
    {
        const char* vertexSource = m_vertexShaderSource.c_str();
        glShaderSource(m_vertexShader, 1, &vertexSource, NULL);
        glCompileShader(m_vertexShader);

        const char* fragmentSource = m_fragmentShaderSource.c_str();
        glShaderSource(m_fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(m_fragmentShader);

        glAttachShader(m_shaderProgram, m_fragmentShader);
        glAttachShader(m_shaderProgram, m_vertexShader);

        glLinkProgram(m_shaderProgram);

        glDeleteShader(m_vertexShader);
        glDeleteShader(m_fragmentShader);

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    }

    Triangle::~Triangle()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteProgram(m_shaderProgram);
    }

    void Triangle::draw()
    {
        glUseProgram(m_shaderProgram);
        glBindVertexArray(m_VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}
