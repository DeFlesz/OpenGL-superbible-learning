#pragma once

class ElementBuffer
{
  private:
    unsigned int rendererID;
    unsigned int count;
  public:
    ElementBuffer(const unsigned int* data, unsigned int count);
    ~ElementBuffer();

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetCount() { return count; }
};