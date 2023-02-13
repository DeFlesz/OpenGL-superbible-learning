#include <iostream>
#include <cmath>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define WIDTH 800
#define HEIGHT 600

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
    GLuint vertex_array_object;
    glCreateVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    glUseProgram(rendering_program);

    // rendering loop
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT);

        // const GLfloat color[] = {
        //     (float)sin(currentTime) * 0.5f + 0.5f,
        //     (float)cos(currentTime) * 0.5f + 0.5f,
        //     0.0f,
        //     1.0f};

        // const GLfloat color2[] = {
        //     0.0f,
        //     (float)sin(currentTime) * 0.5f + 0.5f,
        //     (float)cos(currentTime) * 0.5f + 0.5f,
        //     1.0f};

        const GLfloat attrib[] = {
            (float)sin(currentTime) * 0.5f,
            (float)cos(currentTime) * 0.5f,
            0.0f,
            0.0f};

        glVertexAttrib4fv(0, attrib);
        // glVertexAttrib4fv(1, color2);

        // glClearBufferfv(GL_COLOR, 0, color);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        processInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    glDeleteVertexArrays(1, &vertex_array_object);
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

            layout (location = 0) in vec4 offset;
            // layout (location = 1) in vec4 color;

            out vec4 vs_color;

            void main(void)
            {
                const vec4 vertices[3] = vec4[3](
                    vec4(-0.25, -0.25, 0.5, 1.0),
                    vec4(0.25, -0.25, 0.5, 1.0),
                    vec4(0.0, 0.25, 0.5, 1.0)
                );

                const vec4 colors[] = vec4[3](
                    vec4(1.0, 0.0, 0.0, 1.0),
                    vec4(0.0, 1.0, 0.0, 1.0),
                    vec4(0.0, 0.0, 1.0, 1.0)
                );

                gl_Position = vertices[gl_VertexID] + offset;

                vs_color = colors[gl_VertexID];
            }
        )"};

    static const GLchar *fragment_shader_source[] =
        {
            R"(#version 450 core

            in vec4 vs_color;

            out vec4 color;

            void main(void)
            {
                // color = vec4(
                //     sin(gl_FragCoord.x * 0.25) * 0.5 + 0.5,
                //     cos(gl_FragCoord.y * 0.25) * 0.5 + 0.5,
                //     sin(gl_FragCoord.x * 0.15) * cos(gl_FragCoord.y * 0.15),
                //     1.0
                // );
                color = vs_color;
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