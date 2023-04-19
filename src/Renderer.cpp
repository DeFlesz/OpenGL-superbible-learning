#include "Renderer.h"

void GLClearError() 
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function << "  " << file << ":" << line << std::endl;
        return false;
    }

    return true;
}

void Renderer::Draw(const VertexArrayObject& va, const ElementBuffer& eb, Shader& shader, float time) const 
{
        shader.Bind();

        va.Bind();
        eb.Bind();

        glClear(GL_COLOR_BUFFER_BIT);
        glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0, 0);

        float f = (float) time * (float) M_PI * 0.1;
        glm::mat4 projection = glm::perspective(45.0, 800.0/600.0, 0.1, 100.0);
        glm::mat4 View = glm::mat4(1.0f);
        // trans = glm::vec3(0.0, 0.0, -4.0) * glm::vec3(sinf(2.1 * f) * 0.5, cosf(1.7 * f), sinf(1.3 * f) * cosf(1.5 * f) * 2.0f)

        View = glm::translate(glm::identity<glm::mat4>(), glm::vec3(0.0, 0.0, -4.0)) * 
        glm::translate(glm::identity<glm::mat4>(), glm::vec3(sinf(2.1 * f) * 0.5, cosf(1.7 * f), sinf(1.3 * f) * cosf(1.5 * f) * 2.0f));
        View = glm::rotate(View, (float)(time * 0.45), glm::vec3(0.0, 1.0, 0.0));
        View = glm::rotate(View, (float)(time * 0.81), glm::vec3(1.0, 0.0, 0.0));

        shader.SetUniformMatrix4fv("projection", projection);
        shader.SetUniformMatrix4fv("transform", View);

        GLCall(glDrawElements(GL_TRIANGLES, eb.GetCount(), GL_UNSIGNED_INT, 0));
}