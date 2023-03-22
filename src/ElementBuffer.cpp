#include "ElementBuffer.h"

#include "Renderer.h"

ElementBuffer::ElementBuffer(const unsigned int* data, unsigned int count)
  : count(count)
{
  GLCall(glCreateBuffers(1, &rendererID));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

ElementBuffer::~ElementBuffer()
{
  GLCall(glDeleteBuffers(1, &rendererID));
}

void ElementBuffer::Bind() const
{
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
}

void ElementBuffer::Unbind() const
{
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}