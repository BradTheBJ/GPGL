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

    // Constructor reads and builds the shader
    Shader(const std::filesystem::path& vertexPath,
           const std::filesystem::path& fragmentPath);

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
};
} // namespace gpgl