#pragma once

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VertexArrayObject.h"
#include "ElementBuffer.h"
#include "Shader.h"

#ifdef __linux__
  #include <signal.h>
  #define ASSERT(x) if (!(x)) raise(SIGTRAP)
#elif _WIN32
  #define ASSERT(x) if (!(x)) __debugbreak
#endif

#define GLCall(x) GLClearError();\ 
    x;\ 
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
// yea... I follow the cherno tutorials how do you know?

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class Renderer{
  public:
    void Draw(const VertexArrayObject& va, const ElementBuffer& ib, Shader& shader, float time) const;
};
