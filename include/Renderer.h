#pragma once

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define ASSERT(x) if (!(x)) __debugbreak()
#define GLCall(x) GLClearError();\ 
    x;\ 
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
// yea... I follow the cherno tutorials how do you know?

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);
