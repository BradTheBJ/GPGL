#include "triangle.h"
#include "GLFW/glfw3.h"
#include <string>

namespace gpgl {
Triangle::Triangle(const float &base, const float &height, Window &window)
    : m_vertices{
          m_x,
          m_y + height / (window.getHeight() / 2.0f),
          0.0f, // top
          m_x - base / (window.getWidth() / 2.0f),
          m_y - height / (window.getHeight() / 2.0f),
          0.0f, // bottom left
          m_x + base / (window.getWidth() / 2.0f),
          m_y - height / (window.getHeight() / 2.0f),
          0.0f // bottom right
      },
      m_pWindow(&window), m_base(base), m_height(height) {
  if (base < 0.0f) {
    throw std::invalid_argument("base must be greater than 0.0");
  }
  if (height < 0.0f) {
    throw std::invalid_argument("height must be greater than 0.0");
  }
  m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const char *vertexSource = m_vertexShaderSource.c_str();
  glShaderSource(m_vertexShader, 1, &vertexSource, nullptr);
  glCompileShader(m_vertexShader);

  m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fragmentSource = m_fragmentShaderSource.c_str();
  glShaderSource(m_fragmentShader, 1, &fragmentSource, nullptr);
  glCompileShader(m_fragmentShader);

  m_shaderProgram = glCreateProgram();
  glAttachShader(m_shaderProgram, m_vertexShader);
  glAttachShader(m_shaderProgram, m_fragmentShader);
  glLinkProgram(m_shaderProgram);

  glDeleteShader(m_vertexShader);
  glDeleteShader(m_fragmentShader);

  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);

  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

  // Initial vertex data (will be refined by updateVertices)
  glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float),
               m_vertices.data(), GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Apply initial pixel-to-NDC mapping
  updateVertices();
}

Triangle::~Triangle() {
  glDeleteVertexArrays(1, &m_VAO);
  glDeleteBuffers(1, &m_VBO);
  glDeleteProgram(m_shaderProgram);
}

void Triangle::draw() {
  glUseProgram(m_shaderProgram);
  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

const float &Triangle::getPositionX() const { return m_x; }
const float &Triangle::getPositionY() const { return m_y; }

void Triangle::setPosition(const float &x, const float &y) {
  m_x = x;
  m_y = y;
  updateVertices();
}

void Triangle::updateVertices() {
  if (!m_pWindow)
    return;

  float width = static_cast<float>(m_pWindow->getWidth());
  float height = static_cast<float>(m_pWindow->getHeight());

  // Map pixel coordinates to NDC
  float ndcX = (m_x / (width / 2.0f)) - 1.0f;
  float ndcY = 1.0f - (m_y / (height / 2.0f));

  float ndcBase = m_base / (width / 2.0f);
  float ndcHeight = m_height / (height / 2.0f);

  m_vertices = {
      ndcX,                  ndcY + ndcHeight / 2.0f, 0.0f, // top
      ndcX - ndcBase / 2.0f, ndcY - ndcHeight / 2.0f, 0.0f, // bottom left
      ndcX + ndcBase / 2.0f, ndcY - ndcHeight / 2.0f, 0.0f  // bottom right
  };

  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(float),
                  m_vertices.data());
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
} // namespace gpgl
