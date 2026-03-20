#pragma once
#include "shader.h"
#include <array>
#include <glad/glad.h>
#include "window.h"

namespace gpgl
{
    class Triangle
    {
        public:
        Triangle();
        ~Triangle();

        void draw();

        private:
        Shader m_vertexShaderSource{"../shaders/vertexShader.glsl"};
        Shader m_fragmentShaderSource{"../shaders/fragShader.glsl"};
        std::array<GLfloat, 9> m_vertices;
        GLuint m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        GLuint m_shaderProgram = glCreateProgram();
        GLuint m_VBO;
        GLuint m_VAO;
    };
}
