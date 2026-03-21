#include "rectangle.h"

namespace gpgl {
Rectangle::Rectangle(const float &width, const float &height, Window &window)
    : m_vertices{-width / (window.getWidth() / 2.0f),
                 -height / (window.getHeight() / 2.0f),
                 0.0f,
                 width / (window.getWidth() / 2.0f),
                 -height / (window.getHeight() / 2.0f),
                 0.0f,
                 width / (window.getWidth() / 2.0f),
                 height / (window.getHeight() / 2.0f),
                 0.0f,
                 -width / (window.getWidth() / 2.0f),
                 height / (window.getHeight() / 2.0f),
                 0.0f},
      m_indices{0, 1, 2, 0, 2, 3}, m_width(width), m_height(height),
      m_pWindow(&window) {
  m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const char *vs = m_vertexShaderSource.c_str();
  glShaderSource(m_vertexShader, 1, &vs, nullptr);
  glCompileShader(m_vertexShader);

  m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fs = m_fragmentShaderSource.c_str();
  glShaderSource(m_fragmentShader, 1, &fs, nullptr);
  glCompileShader(m_fragmentShader);

  m_shaderProgram = glCreateProgram();
  glAttachShader(m_shaderProgram, m_vertexShader);
  glAttachShader(m_shaderProgram, m_fragmentShader);
  glLinkProgram(m_shaderProgram);

  glDeleteShader(m_vertexShader);
  glDeleteShader(m_fragmentShader);

  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);
  glGenBuffers(1, &m_EBO);

  glBindVertexArray(m_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float),
               m_vertices.data(), GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint),
               m_indices.data(), GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  // Apply initial pixel-to-NDC mapping
  updateVertices();
}

Rectangle::~Rectangle() {
  glDeleteVertexArrays(1, &m_VAO);
  glDeleteBuffers(1, &m_VBO);
  glDeleteBuffers(1, &m_EBO);
  glDeleteProgram(m_shaderProgram);
}

void Rectangle::draw() {
  glUseProgram(m_shaderProgram);
  glBindVertexArray(m_VAO);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Rectangle::setPosition(const float &x, const float &y) {
  m_x = x;
  m_y = y;
  updateVertices();
}

const float &Rectangle::getPositionX() const { return m_x; }
const float &Rectangle::getPositionY() const { return m_y; }

void Rectangle::updateVertices() {
  if (!m_pWindow)
    return;

  float width = static_cast<float>(m_pWindow->getWidth());
  float height = static_cast<float>(m_pWindow->getHeight());

  // Map pixel coordinates to NDC
  float ndcX = (m_x / (width / 2.0f)) - 1.0f;
  float ndcY = 1.0f - (m_y / (height / 2.0f));

  float ndcHalfWidth = m_width / width;
  float ndcHalfHeight = m_height / height;

  // Recalculate vertices centered at (ndcX, ndcY)
  m_vertices = {
      ndcX - ndcHalfWidth, ndcY - ndcHalfHeight, 0.0f, // bottom left
      ndcX + ndcHalfWidth, ndcY - ndcHalfHeight, 0.0f, // bottom right
      ndcX + ndcHalfWidth, ndcY + ndcHalfHeight, 0.0f, // top right
      ndcX - ndcHalfWidth, ndcY + ndcHalfHeight, 0.0f  // top left
  };

  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(float),
                  m_vertices.data());
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
} // namespace gpgl
