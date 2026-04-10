#pragma once
#include <filesystem>
#include <glad/glad.h>
#include <string>
#include <string_view>

namespace gpgl {
class Shader {
  public:
    // Shader program ID
    unsigned int ID;

    // Constructor reads and builds the shader from files
    Shader(const std::filesystem::path& vertexPath,
           const std::filesystem::path& fragmentPath);

    // Constructor for fragment shader from file and vertex shader from string
    Shader(std::string_view vertexCode, const std::filesystem::path& fragmentPath);

    // Constructor for fragment shader from file using default vertex shader
    Shader(const std::filesystem::path& fragmentPath);

    // Use/activate the shader
    void use() const;

    // Utility uniform functions
    void setBool(std::string_view name, const bool& value) const;
    void setInt(std::string_view name, const int& value) const;
    void setFloat(std::string_view name, const float& value) const;

  private:
    std::string m_vertexCode;
    std::string m_fragmentCode;
    unsigned int m_vertexShader;
    unsigned int m_fragmentShader;
    int m_success;
    char m_infoLog[512];

    void compileAndLink();

    static constexpr std::string_view s_defaultVertexSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform float u_x;
uniform float u_y;
uniform float u_rotation;
uniform float u_windowWidth;
uniform float u_windowHeight;

void main()
{
    float cosT = cos(u_rotation);
    float sinT = sin(u_rotation);
    
    // Rotate 
    vec2 rotatedPos;
    rotatedPos.x = aPos.x * cosT - aPos.y * sinT;
    rotatedPos.y = aPos.x * sinT + aPos.y * cosT;
    
    // Translate
    vec2 finalPixelPos = rotatedPos + vec2(u_x, u_y);
    
    // Convert to NDC
    float ndcX = (finalPixelPos.x / (u_windowWidth / 2.0)) - 1.0;
    float ndcY = 1.0 - (finalPixelPos.y / (u_windowHeight / 2.0));
    
    gl_Position = vec4(ndcX, ndcY, 0.0, 1.0);
}
)";
};
} // namespace gpgl