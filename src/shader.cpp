#include "shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace gpgl {

Shader::Shader(const std::filesystem::path& vertexPath,
               const std::filesystem::path& fragmentPath) {
    // 1. Read shader files into strings
    std::ifstream vShaderFile(vertexPath);
    std::ifstream fShaderFile(fragmentPath);

    if (!vShaderFile.is_open() || !fShaderFile.is_open()) {
        throw std::runtime_error("Failed to open shader file(s)");
    }

    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    m_vertexCode = vShaderStream.str();
    m_fragmentCode = fShaderStream.str();

    compileAndLink();
}

Shader::Shader(std::string_view vertexCode,
               const std::filesystem::path& fragmentPath) {
    std::ifstream fShaderFile(fragmentPath);

    if (!fShaderFile.is_open()) {
        throw std::runtime_error("Failed to open fragment shader file");
    }

    std::stringstream fShaderStream;
    fShaderStream << fShaderFile.rdbuf();

    m_vertexCode = std::string(vertexCode);
    m_fragmentCode = fShaderStream.str();

    compileAndLink();
}

Shader::Shader(const std::filesystem::path& fragmentPath)
    : Shader(s_defaultVertexSource, fragmentPath) {}

Shader::Shader(std::string_view vertexCode, std::string_view fragmentCode) {
    m_vertexCode = std::string(vertexCode);
    m_fragmentCode = std::string(fragmentCode);
    compileAndLink();
}

Shader::Shader() : Shader(s_defaultVertexSource, s_defaultFragmentSource) {}

void Shader::compileAndLink() {
    const char* vShaderCode = m_vertexCode.c_str();
    const char* fShaderCode = m_fragmentCode.c_str();

    // 2. Compile vertex shader
    m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_vertexShader, 1, &vShaderCode, nullptr);
    glCompileShader(m_vertexShader);

    glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &m_success);
    if (!m_success) {
        glGetShaderInfoLog(m_vertexShader, 512, nullptr, m_infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << m_infoLog << std::endl;
    }

    // 3. Compile fragment shader
    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_fragmentShader, 1, &fShaderCode, nullptr);
    glCompileShader(m_fragmentShader);

    glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &m_success);
    if (!m_success) {
        glGetShaderInfoLog(m_fragmentShader, 512, nullptr, m_infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << m_infoLog << std::endl;
    }

    // 4. Link shaders into a program
    ID = glCreateProgram();
    glAttachShader(ID, m_vertexShader);
    glAttachShader(ID, m_fragmentShader);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &m_success);
    if (!m_success) {
        glGetProgramInfoLog(ID, 512, nullptr, m_infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << m_infoLog << std::endl;
    }

    // 5. Delete shader objects after linking
    glDeleteShader(m_vertexShader);
    glDeleteShader(m_fragmentShader);
}

void Shader::use() const { glUseProgram(ID); }

void Shader::setBool(std::string_view name, const bool& value) const {
    glUniform1i(glGetUniformLocation(ID, std::string(name).c_str()),
                static_cast<int>(value));
}

void Shader::setInt(std::string_view name, const int& value) const {
    glUniform1i(glGetUniformLocation(ID, std::string(name).c_str()), value);
}

void Shader::setFloat(std::string_view name, const float& value) const {
    glUniform1f(glGetUniformLocation(ID, std::string(name).c_str()), value);
}

} // namespace gpgl