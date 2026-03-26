#pragma once
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

using fs = std::filesystem::path;

namespace gpgl {
// Reads a GLSL shader file from disk and holds the source as a string.
// The source is passed directly to glShaderSource at compile time.
struct Shader {
    std::string source;

    Shader(const fs& shaderFile) {
        std::ifstream file(shaderFile);
        if (!file.is_open())
            throw std::runtime_error("Failed to open shader file: " +
                                     shaderFile.string());

        std::stringstream buffer;
        buffer << file.rdbuf();
        source = buffer.str();
    }

    const char* c_str() const { return source.c_str(); }
};
} // namespace gpgl