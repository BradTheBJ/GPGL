#include "triangle.h"
#include <string>
#include "GLFW/glfw3.h"

namespace gpgl
{
    Triangle::Triangle(const float& base, const float& height, Window& window)
    : m_vertices{
        0.0f, height / (window.getHeight() / 2.0f), 0.0f, // top
       -base / (window.getWidth() / 2.0f), -height / (window.getHeight() / 2.0f), 0.0f, // bottom left
        base / (window.getWidth() / 2.0f), -height / (window.getHeight() / 2.0f), 0.0f  // bottom right
   }
    {
        if (base < 0.0f)
        {
            throw std::invalid_argument("base must be greater than 0.0");
        }
        if (height < 0.0f)
        {
            throw std::invalid_argument("width must be greater than 0.0");
        }
        m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* vertexSource = m_vertexShaderSource.c_str();
        glShaderSource(m_vertexShader, 1, &vertexSource, nullptr);
        glCompileShader(m_vertexShader);

        m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragmentSource = m_fragmentShaderSource.c_str();
        glShaderSource(m_fragmentShader, 1, &fragmentSource, nullptr);
        glCompileShader(m_fragmentShader);

        m_shaderProgram = glCreateProgram();
        glAttachShader(m_shaderProgram, m_vertexShader);
        glAttachShader(m_shaderProgram, m_fragmentShader);
        glLinkProgram(m_shaderProgram);

        glDeleteShader(m_vertexShader);
        glDeleteShader(m_fragmentShader);

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        glBufferData(GL_ARRAY_BUFFER,
                     m_vertices.size() * sizeof(float),
                     m_vertices.data(),
                     GL_DYNAMIC_DRAW);

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
