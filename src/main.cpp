#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

#include "Renderer.h"

#include "VertexArrayObject.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

#define WIDTH 800
#define HEIGHT 600


struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

struct Vertex
{
    glm::vec3 position;
    glm::vec4 color;
};

static unsigned int CompileShader(unsigned int type, std::string source)
{
    unsigned int shaderID = glCreateShader(type);
    const char* sourcePtr = source.c_str();

    glShaderSource(shaderID, 1, &sourcePtr, NULL);
    glCompileShader(shaderID);

    int result;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];

        glGetShaderInfoLog(shaderID, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<" shader!" << message << std::endl;

        glDeleteShader(shaderID);
        delete [] message;
        return 0;
    }

    return shaderID;
}

static unsigned int CreateShader(std::string& vertexSource, std::string& fragmentSource)
{
    unsigned int program = glCreateProgram();

    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glValidateProgram(program);

//  glDetachShader <- good call but it render debugging capabilites useless
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

static ShaderProgramSource ParseShader(const std::string& filepath) 
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

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void resize_window_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);

    glm::mat4 projection = glm::perspective(45.0, ((double)width)/((double)height), 0.1, 100.0);
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(projection));
}

int main()
{
    // setup
    GLFWwindow *window;

    glfwInit();

    window = glfwCreateWindow(WIDTH, HEIGHT, "Okno", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        std::cout << "fail...\n";
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, resize_window_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    // after setup
    // GLuint rendering_program = compile_shaders();
    // GLuint rendering_program2 = compile_shaders(1);
    ShaderProgramSource source = ParseShader("../res/shaders/VertexColor.glsl");

    unsigned int rendering_program = CreateShader(source.vertexSource, source.fragmentSource);
    // use shader program
    glUseProgram(rendering_program);

    const Vertex vertices[] = {
        {glm::vec3(-0.25, -0.25, 0.25), glm::vec4(1.0, 1.0, 1.0, 1.0)}, // 0 - lewo, dół, przód
        {glm::vec3(0.25, -0.25, 0.25), glm::vec4(0.0, 1.0, 0.0, 1.0)}, // 1 - prawo, dół, przód
        {glm::vec3(-0.25, 0.25, 0.25), glm::vec4(1.0, 0.0, 0.0, 1.0)}, // 2 - lewo, góra, przód
        {glm::vec3(0.25, 0.25, 0.25), glm::vec4(1.0, 1.0, 0.0, 1.0)}, // 3 - prawo, góra, przód
        {glm::vec3(-0.25, -0.25, -0.25), glm::vec4(0.0, 0.0, 1.0, 1.0)}, // 4 - lewo, dół, tył
        {glm::vec3(0.25, -0.25, -0.25), glm::vec4(0.0, 1.0, 1.0, 1.0)}, // 5 - prawo, dół, tył
        {glm::vec3(-0.25, 0.25, -0.25), glm::vec4(1.0, 0.0, 1.0, 1.0)}, // 6 - lewo, góra, tył
        {glm::vec3(0.25, 0.25, -0.25), glm::vec4(0.0, 0.0, 0.0, 1.0)}, // 7 - prawo, góra, tył
    };

    const unsigned int indices[] = {
        // top
        2, 6, 7,
        2, 3, 7,
        // bottom
        0, 4, 5,
        0, 1, 5,

        // left
        0, 2, 6,
        0, 4, 6,

        // right
        1, 3, 7,
        1, 5, 7,

        // front
        0, 2, 3,
        0, 1, 3,

        // back
        4, 6, 7,
        4, 5, 7
    };

    VertexArrayObject vertexArrayObject;


    // create a buffer so we can push those vertices to the target
    VertexBuffer vertexBuffer(vertices, sizeof(vertices));

    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(4);

    vertexArrayObject.AddBuffer(vertexBuffer, layout);

    // create EBO
    ElementBuffer elementBuffer(indices, std::size(indices));

    GLuint transformLocation = glGetUniformLocation(rendering_program, "transform");
    glm::mat4 trans = glm::mat4(1.0f);

    GLuint projectionLocation = glGetUniformLocation(rendering_program, "projection");
    glm::mat4 projection = glm::perspective(45.0, 800.0/600.0, 0.1, 100.0);
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

// to be enabled when i will have faces to cull
    // glEnable(GL_CULL_FACE);
    // glFrontFace(GL_CCW);

    // rendering loop
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT);
        glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0, 0);
        // trans = glm::rotate(trans, (float)currentTime / 1000, glm::vec3(0.0, 1.0, 0.0));
        float f = (float) currentTime * (float) M_PI * 0.1;
        
        // trans = glm::vec3(0.0, 0.0, -4.0) * glm::vec3(sinf(2.1 * f) * 0.5, cosf(1.7 * f), sinf(1.3 * f) * cosf(1.5 * f) * 2.0f)

        glm::mat4 View = glm::translate(glm::identity<glm::mat4>(), glm::vec3(0.0, 0.0, -4.0)) * 
        glm::translate(glm::identity<glm::mat4>(), glm::vec3(sinf(2.1 * f) * 0.5, cosf(1.7 * f), sinf(1.3 * f) * cosf(1.5 * f) * 2.0f));
        View = glm::rotate(View, (float)(currentTime * 0.45), glm::vec3(0.0, 1.0, 0.0));
        View = glm::rotate(View, (float)(currentTime * 0.81), glm::vec3(1.0, 0.0, 0.0));
        

        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(View));

        GLCall(glDrawElements(GL_TRIANGLES, elementBuffer.GetCount(), GL_UNSIGNED_INT, 0));

        processInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    glDeleteProgram(rendering_program);

    return 0;
}