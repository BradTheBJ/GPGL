#pragma once
#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <filesystem>
#include <string_view>
#include <fstream>

namespace gpgl {
    class Shader
    {
    public:
        // the program ID
        unsigned int ID;

        // constructor reads and builds the shader
        Shader(std::filesystem::path vertexPath, std::filesystem::path fragmentPath);
        // use/activate the shader
        void use();
        // utility uniform functions
        void setBool(std::string_view name, const bool& value) const;
        void setInt(std::string_view name, const int& value) const;
        void setFloat(std::string_view name, const float& value) const;
    };
}
