#pragma once

#include <iostream>
#include <unordered_map>

#include "Renderer.h"

struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader {
  private:
    std::string filePath;
    unsigned int rendererID;
    std::unordered_map<std::string, int> uniformLocationCache;

  public:
    Shader(const std::string& filepath);

    ~Shader();


    void Bind() const;
    void Unbind() const;

    void SetUniformMatrix4fv(const std::string& name, glm::mat4& mat);

  private:
    unsigned int CreateShader(std::string& vertexSource, std::string& fragmentSource);
    unsigned int CompileShader(unsigned int type, std::string source);
    ShaderProgramSource ParseShader(const std::string& filepath);
    int GetUniformLocation(const std::string& name);  
};