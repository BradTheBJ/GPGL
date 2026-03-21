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

        const float& getPositionX() const;
        const float& getPositionY() const;

        void setPosition(const float& x, const float& y);
        //const float& getWidth() const;
        //const float& getHeight() const;

        private:
        Shader m_vertexShaderSource{"../shaders/vertexShader.glsl"};
        Shader m_fragmentShaderSource{"../shaders/fragShader.glsl"};
        std::array<GLfloat, 9> m_vertices;
        GLuint m_vertexShader = 0;
        GLuint m_fragmentShader = 0;
        GLuint m_shaderProgram = 0;
        GLuint m_VBO = 0;
        GLuint m_VAO = 0;
        GLfloat m_x = 0.0f, m_y = 0.0f;
        Window* m_pWindow = nullptr;
        float m_base = 0.0f, m_height = 0.0f;
        void updateVertices();
    };
}
