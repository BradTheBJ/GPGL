#pragma once
#include "shader.h"
#include <glad/glad.h>
#include <array>

namespace gpgl
{
    class Rectangle
    {
        public:
        Rectangle();
        ~Rectangle();

        void draw();

        private:
        Shader m_vertexShaderSource{"../shaders/vertexShader.glsl"};
        Shader m_fragmentShaderSource{"../shaders/fragShader.glsl"};
        GLuint m_vertexShader = 0;
        GLuint m_fragmentShader = 0;
        GLuint m_shaderProgram = 0;
        GLuint m_VBO = 0;
        GLuint m_VAO = 0;
        GLuint m_EBO = 0;
        std::array<GLfloat, 12> m_vertices;
        std::array<GLuint, 6> m_indices;
    };
}
