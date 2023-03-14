#pragma once

#include <glad/glad.h>

#include <iostream>
#include <vector>

#include "Renderer.h"

struct VertexBufferElement
{
  unsigned int type;
  unsigned int count;
  unsigned char normalized;

  static unsigned int GetSizeOfType(unsigned int type)
  {
    switch (type)
    {
      case GL_FLOAT:
        return 4;
      case GL_UNSIGNED_INT:
        return 4;
      case GL_UNSIGNED_BYTE:
        return 1;
    }
    ASSERT(false);
    return 0;
  }
};

class VertexBufferLayout
{
  private:
    std::vector<VertexBufferElement> elements;
    unsigned int stride;

  public:
    // VertexBufferLayout();
    VertexBufferLayout() : stride(0) {  }

    template <typename T>
    void Push(unsigned int count);

    inline const std::vector<VertexBufferElement> GetElements() const { return elements; }
    inline unsigned int GetStride() const { return stride; }
};