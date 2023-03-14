#include "VertexArrayObject.h"

#include "Renderer.h"

VertexArrayObject::VertexArrayObject()
{
  GLCall(glCreateVertexArrays(1, &rendererID));
}

VertexArrayObject::~VertexArrayObject()
{
  GLCall(glDeleteVertexArrays(1, &rendererID));
}

void VertexArrayObject::Bind() const
{
  GLCall(glBindVertexArray(rendererID));
}

void VertexArrayObject::Unbind() const
{
  GLCall(glBindVertexArray(0));
}

void VertexArrayObject::AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout)
{
  vertexBuffer.Bind();

  const auto& elements = layout.GetElements();
  unsigned int offset = 0;
  for (unsigned int i = 0; i < elements.size(); i++)
  {
    const auto& element = elements[i];
    GLCall(glEnableVertexAttribArray(i));
    GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
    offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
  }
}