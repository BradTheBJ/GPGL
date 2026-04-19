#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stbImage.h"
#include <iostream>

namespace gpgl {

Texture::Texture(const std::filesystem::path& filePath)
    : m_path(std::filesystem::path(GPGL_ASSETS_DIR) / filePath) {
    // Flip textures vertically on load by default for OpenGL
    stbi_set_flip_vertically_on_load(true);

    m_data = stbi_load(m_path.c_str(), &m_width, &m_height, &m_channels, 4);

    if (!m_data) {
        std::cerr << "Failed to load texture: " << m_path << std::endl;
        return;
    }

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    // Set texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(m_data);
    m_data = nullptr;
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    if (m_id) {
        glDeleteTextures(1, &m_id);
    }
}

void Texture::bind(unsigned int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace gpgl