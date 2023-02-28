#include <iostream>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

void resize_window_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GLuint compile_shaders(int type = 0);

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
    GLuint rendering_program = compile_shaders();
    GLuint rendering_program2 = compile_shaders(1);
    // use shader program
    glUseProgram(rendering_program);

    const Vertex vertices[] = {
        {glm::vec3(-0.25, 0, 0.0), glm::vec4(1.0, 0.0, 1.0, 1.0)},
        {glm::vec3(0.25, 0, 0.0), glm::vec4(1.0, 1.0, 0.1, 1.0)},
        {glm::vec3(0.0, 0.5, 0.0), glm::vec4(1.0, 0.0, 0.0, 1.0)},
        {glm::vec3(-0.5, -0.5, 0.0), glm::vec4(0.0, 0.0, 1.0, 1.0)},
        {glm::vec3(0.0, -0.5, 0.0), glm::vec4(0.0, 1.0, 1.0, 1.0)},
        {glm::vec3(0.5, -0.5, 0.0), glm::vec4(0.0, 1.0, 0.0, 1.0)},
    };
    // TODO they are actually indices
    const GLuint elements[] = {
        0, 1, 2,
        3, 4, 0,
        4, 5, 1};

    // create a vao for opengl to put vertices in
    GLuint VertexArrayObject;
    glCreateVertexArrays(1, &VertexArrayObject);
    glBindVertexArray(VertexArrayObject);

    // create a buffer so we can push those vertices to the target
    GLuint VertexBufferObject;
    glCreateBuffers(1, &VertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // create EBO
    GLuint VertexElementBuffer;
    glCreateBuffers(1, &VertexElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VertexElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    // define position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

    // define color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));

    GLuint transformLoacation = glGetUniformLocation(rendering_program, "transform");
    glm::mat4 trans = glm::mat4(1.0f);
    // rendering loop
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT);
        trans = glm::rotate(trans, (float)currentTime / 1000, glm::vec3(0.0, 1.0, 0.0));
        glUniformMatrix4fv(transformLoacation, 1, GL_FALSE, glm::value_ptr(trans));

        glDrawElements(GL_TRIANGLES, std::size(elements), GL_UNSIGNED_INT, 0);

        processInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    glDeleteVertexArrays(1, &VertexArrayObject);
    glDeleteProgram(rendering_program);

    return 0;
}

GLuint compile_shaders(int type)
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

            uniform mat4 transform;

            void main()
            {
                gl_Position =  transform * vec4(position, 1.0);

                vs_color = color;
            }
        )"};

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    // delete[] *vertex_shader_source;

    if (type)
    {
        static const GLchar *fragment_shader_source[] =
            {
                R"(#version 450 core

            in vec4 vs_color;

            out vec4 FragColor;

            void main()
            {
                FragColor = vec4(1.0, 1.0, 0.0, 1.0);
            }
        )"};

        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
        glCompileShader(fragment_shader);

        // delete[] *fragment_shader_source;
    }
    else
    {
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

        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
        glCompileShader(fragment_shader);

        // delete[] *fragment_shader_source;
    }

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}