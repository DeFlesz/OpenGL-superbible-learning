#include "Shader.h"

#include <fstream>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string& filepath) : filePath(filepath)
{  
  ShaderProgramSource source = ParseShader(filepath);
  // std::cout << source.vertexSource << "\n" << source.fragmentSource << std::endl;
  rendererID = CreateShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader() 
{ 
  GLCall(glDeleteProgram(rendererID));
}

unsigned int Shader::CompileShader(unsigned int type, std::string source) {
  unsigned int shaderID = glCreateShader(type);
  const char* sourcePtr = source.c_str();
  
  GLCall(glShaderSource(shaderID, 1, &sourcePtr, NULL));
  GLCall(glCompileShader(shaderID));

  int result;
  GLCall(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result));
  if (result == GL_FALSE)
  {
      int length;
      GLCall(glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length));
      char* message = new char[length];

      GLCall(glGetShaderInfoLog(shaderID, length, &length, message));
      std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<" shader!" << message << std::endl;

      GLCall(glDeleteShader(shaderID));
      delete [] message;
      return 0;
  }
  // std::cout << shaderID << std::endl;
  return shaderID;
}

unsigned int Shader::CreateShader(std::string& vertexSource, std::string& fragmentSource)
{
  GLCall(unsigned int program = glCreateProgram());
  // std::cout << vertexSource << "\n" << fragmentSource << std::endl;

  unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
  unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
  // std::cout << program << "\n" << vertexShader << "\n" << fragmentShader << std::endl;
  
  GLCall(glAttachShader(program, vertexShader));
  GLCall(glAttachShader(program, fragmentShader));
  GLCall(glLinkProgram(program));
  GLCall(glValidateProgram(program));

// glDetachShader <- good call but it render debugging capabilites useless
  GLCall(glDeleteShader(vertexShader));
  GLCall(glDeleteShader(fragmentShader));

  return program;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) 
{
  std::ifstream stream(filepath);

  enum class ShaderType
  {
      NONE = -1, VERTEX = 0, FRAGMENT = 1
  };

  std::string line;
  std::stringstream stringStreams[2];
  ShaderType shaderType = ShaderType::NONE;
  while (getline(stream, line))
  {
      if (line.find("#shader") != std::string::npos)
      {
          if (line.find("vertex") != std::string::npos)
          {
              shaderType = ShaderType::VERTEX;
          }
          else if (line.find("fragment") != std::string::npos)
          {
              shaderType = ShaderType::FRAGMENT;
          }
      }
      else
      {
          stringStreams[(int)shaderType] << line << "\n";
      }
  }

  return 
  { 
      stringStreams[(int)ShaderType::VERTEX].str(),
      stringStreams[(int)ShaderType::FRAGMENT].str()
  };
}

void Shader::Bind() const
{
  GLCall(glUseProgram(rendererID));
}

void Shader::Unbind() const
{
  GLCall(glUseProgram(0));
}

void Shader::SetUniformMatrix4fv(const std::string& name, glm::mat4 &mat)
{
    // std::cout << glm::value_ptr(mat) << std::endl;
  GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat)));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    // std::cout << glm::value_ptr(mat) << std::endl;
  GLCall(glUniform1i(GetUniformLocation(name), value));
}

int Shader::GetUniformLocation(const std::string& name) {
  if (uniformLocationCache.find(name) != uniformLocationCache.end())
    return uniformLocationCache[name];

  GLCall(int location = glGetUniformLocation(rendererID, name.c_str()));
  if (location == -1)
  {
    std::cout << "Warning: uniform '" << name << "' doesn't exist" << std::endl;
  }
  else
  {
    uniformLocationCache[name] = location;  
  }

  return location;
}