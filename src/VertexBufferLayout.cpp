#include "VertexBufferLayout.h"

template<>
void VertexBufferLayout::Push<float>(unsigned int count)
{
  elements.push_back({GL_FLOAT, count, GL_FALSE});
  stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
}

template<>
void VertexBufferLayout::Push<unsigned int>(unsigned int count)
{
  elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
  stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
}

template<>
void VertexBufferLayout::Push<unsigned char>(unsigned int count)
{
  elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
  stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
}