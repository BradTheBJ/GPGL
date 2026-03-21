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
        Triangle(const float& base, const float& height, Window& window);
        ~Triangle();

        void draw();

        private:
        Shader m_vertexShaderSource{"../shaders/vertexShader.glsl"};
        Shader m_fragmentShaderSource{"../shaders/fragShader.glsl"};
        std::array<GLfloat, 9> m_vertices;
        GLuint m_vertexShader = 0;
        GLuint m_fragmentShader = 0;
        GLuint m_shaderProgram = 0;
        GLuint m_VBO = 0;
        GLuint m_VAO = 0;
    };
}
