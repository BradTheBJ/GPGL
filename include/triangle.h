#pragma once
#include "shader.h"
#include "window.h"
#include <array>
#include <glad/glad.h>

namespace gpgl
{
    class Triangle
    {
    public:
        Triangle(const float& base, const float& height, Window& window);
        ~Triangle();

        void draw();

        void setPosition(const float& x, const float& y);
        const float& getPositionX() const;
        const float& getPositionY() const;

        void setFrag(const std::filesystem::path& path);
        void setVertex(const std::filesystem::path& path);

    private:
        Shader m_vertexShaderSource{"../shaders/vertexShader.glsl"};
        Shader m_fragmentShaderSource{"../shaders/fragShader.glsl"};
        GLuint m_vertexShader   = 0;
        GLuint m_fragmentShader = 0;
        GLuint m_shaderProgram  = 0;
        GLuint m_VBO = 0;
        GLuint m_VAO = 0;
        float m_base = 0.0f, m_height = 0.0f;
        GLfloat m_x = 0.0f, m_y = 0.0f;
        std::array<GLfloat, 9> m_vertices;
        Window* m_pWindow = nullptr;

        void updateVertices();
        void calculateShaders();
    };
} // namespace gpgl
