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

#include "Shader.h"

#include "VertexArrayObject.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

#include "Texture.h"

#define WIDTH 800
#define HEIGHT 600

struct Vertex
{
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 uv;
};

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
    Shader shader("../res/shaders/VertexColor.glsl");
    shader.Bind();

    Texture texture("../res/assets/photo.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    Renderer renderer;

    const Vertex vertices[] = {
        {glm::vec3(-0.25, -0.25, 0.25), glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec2(0.0, 0.0)}, // 0 - lewo, dół, przód
        {glm::vec3(0.25, -0.25, 0.25), glm::vec4(0.0, 1.0, 0.0, 1.0), glm::vec2(1.0, 0.0)}, // 1 - prawo, dół, przód
        {glm::vec3(-0.25, 0.25, 0.25), glm::vec4(1.0, 0.0, 0.0, 1.0), glm::vec2(0.0, 1.0)}, // 2 - lewo, góra, przód
        {glm::vec3(0.25, 0.25, 0.25), glm::vec4(1.0, 1.0, 0.0, 1.0), glm::vec2(1.0, 1.0)}, // 3 - prawo, góra, przód
        {glm::vec3(-0.25, -0.25, -0.25), glm::vec4(0.0, 0.0, 1.0, 1.0), glm::vec2(0.0, 1.0)}, // 4 - lewo, dół, tył
        {glm::vec3(0.25, -0.25, -0.25), glm::vec4(0.0, 1.0, 1.0, 1.0), glm::vec2(1.0, 1.0)}, // 5 - prawo, dół, tył
        {glm::vec3(-0.25, 0.25, -0.25), glm::vec4(1.0, 0.0, 1.0, 1.0), glm::vec2(0.0, 0.0)}, // 6 - lewo, góra, tył
        {glm::vec3(0.25, 0.25, -0.25), glm::vec4(0.0, 0.0, 0.0, 1.0), glm::vec2(1.0, 0.0)}, // 7 - prawo, góra, tył
    };

    const unsigned int indices[] = {
        // top
        2, 7, 6,
        2, 3, 7,
        // bottom
        0, 4, 5,
        0, 5, 1,

        // left
        0, 2, 6,
        0, 6, 4,

        // right
        1, 7, 3,
        1, 5, 7,

        // front
        0, 3, 2,
        0, 1, 3,

        // back
        4, 6, 7,
        4, 7, 5
    };

    VertexArrayObject vertexArrayObject;
    vertexArrayObject.Bind();


    // create a buffer so we can push those vertices to the target
    VertexBuffer vertexBuffer(vertices, sizeof(vertices));

    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(4);
    layout.Push<float>(2);

    vertexArrayObject.AddBuffer(vertexBuffer, layout);

    // create EBO
    ElementBuffer elementBuffer(indices, std::size(indices));
    // std::cout << elementBuffer.GetCount() << std::endl;

    // glm::mat4 trans = glm::mat4(1.0f);

    // shader.Unbind();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // to be enabled when i will have faces to cull
    glEnable(GL_CULL_FACE);
    // glFrontFace(GL_CCW);

    // rendering loop
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();

        renderer.Draw(vertexArrayObject, elementBuffer, shader, currentTime);

        processInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}