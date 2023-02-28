#include <iostream>
#include <cmath>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define WIDTH 800
#define HEIGHT 600

// TODO
// VEO

struct Vertex
{
    glm::vec3 position;
    glm::vec4 color;
};

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

GLuint compile_shaders();

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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    // after setup
    GLuint rendering_program = compile_shaders();
    // use shader program
    glUseProgram(rendering_program);

    const Vertex vertices[3] = {
        {glm::vec3(-0.25, -0.25, 0.5), glm::vec4(1.0, 0.1, 0.1, 1.0)},
        {glm::vec3(0.25, -0.25, 0.5), glm::vec4(0.1, 1.0, 0.1, 1.0)},
        {glm::vec3(0.0, 0.25, 0.5), glm::vec4(0.1, 0.1, 1.0, 1.0)},
    };

    // create a vao for opengl to put vertices in
    GLuint VertexArrayObject;
    glCreateVertexArrays(1, &VertexArrayObject);
    glBindVertexArray(VertexArrayObject);

    // create a buffer so we can push those vertices to the target
    GLuint VertexBufferObject;
    glCreateBuffers(1, &VertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // define position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

    // define color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));

    // rendering loop
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        // glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, std::size(vertices));

        processInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    glDeleteVertexArrays(1, &VertexArrayObject);
    glDeleteProgram(rendering_program);

    return 0;
}

GLuint compile_shaders()
{
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program;

    static const GLchar *vertex_shader_source[] =
        {
            R"(#version 450 core

            layout (location = 0) in vec3 position;
            layout (location = 1) in vec4 color;

            out vec4 vs_color;

            void main()
            {
                gl_Position = vec4(position, 1.0);

                vs_color = color;
            }
        )"};

    static const GLchar *fragment_shader_source[] =
        {
            R"(#version 450 core

            in vec4 vs_color;

            out vec4 FragColor;

            void main()
            {
                FragColor = vs_color;
            }
        )"};

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}