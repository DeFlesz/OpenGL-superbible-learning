#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArrayObject
{
  private:
    unsigned int rendererID;

  public:
    VertexArrayObject();
    ~VertexArrayObject();

    void Bind() const;
    void Unbind() const;

    void AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);
};