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

    glm::mat4 projection = glm::perspective(45.0, ((double)width)/((double)height), 0.1, 100.0);
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(projection));
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
        {glm::vec3(-0.25, -0.25, 0.25), glm::vec4(1.0, 1.0, 1.0, 1.0)}, // 0 - lewo, dół, przód
        {glm::vec3(0.25, -0.25, 0.25), glm::vec4(0.0, 1.0, 0.0, 1.0)}, // 1 - prawo, dół, przód
        {glm::vec3(-0.25, 0.25, 0.25), glm::vec4(1.0, 0.0, 0.0, 1.0)}, // 2 - lewo, góra, przód
        {glm::vec3(0.25, 0.25, 0.25), glm::vec4(1.0, 1.0, 0.0, 1.0)}, // 3 - prawo, góra, przód
        {glm::vec3(-0.25, -0.25, -0.25), glm::vec4(0.0, 0.0, 1.0, 1.0)}, // 4 - lewo, dół, tył
        {glm::vec3(0.25, -0.25, -0.25), glm::vec4(0.0, 1.0, 1.0, 1.0)}, // 5 - prawo, dół, tył
        {glm::vec3(-0.25, 0.25, -0.25), glm::vec4(1.0, 0.0, 1.0, 1.0)}, // 6 - lewo, góra, tył
        {glm::vec3(0.25, 0.25, -0.25), glm::vec4(0.0, 0.0, 0.0, 1.0)}, // 7 - prawo, góra, tył
    };
    // TODO they are actually indices
    const GLuint elements[] = {
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

            layout (location = 0) uniform mat4 projection;
            uniform mat4 transform;

            void main()
            {
                gl_Position = projection * transform * vec4(position, 1.0);

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