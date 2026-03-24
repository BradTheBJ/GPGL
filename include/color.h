#pragma once
#include <array>

#include "window.h"

namespace gpgl {
struct Color {
  float r, g, b, a;

  // Constructor taking 0-255 ints
  Color(const unsigned int &red, const unsigned int &green,
        const unsigned int &blue, const unsigned int &alpha = 255) {
    r = red / 255.0f;
    g = green / 255.0f;
    b = blue / 255.0f;
    a = alpha / 255.0f;
  }
  std::array<float, 4> rgba = {r, g, b, a};
};
} // namespace gpgl
