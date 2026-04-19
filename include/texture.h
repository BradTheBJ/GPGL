#pragma once
#include "stbImage.h"
#include <filesystem>
#include <glad/glad.h>
#include <string>

namespace gpgl {
class Texture {
  public:
    Texture(const std::filesystem::path& filePath);
    ~Texture();

    void bind(unsigned int unit = 0) const;
    void unbind() const;

  private:
    std::filesystem::path m_path;
    int m_width = 0, m_height = 0, m_channels = 0;
    unsigned char* m_data = nullptr;
    GLuint m_id = 0;
};
} // Namespace gpgl