#pragma once

#include <iostream>

class Shader {
  std::string filename;

  public:
    Shader(std::string filename);

    ~Shader();
};