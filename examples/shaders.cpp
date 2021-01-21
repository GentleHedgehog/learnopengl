#include "shaders.h"
#include "shaders/shaders_chapter.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

ShadersExample::ShadersExample()
{    
//    transferColorExample();
    uniformColorExample();
}

void ShadersExample::transferColorExample()
{
    sp.createAndLink(transferVarVS, transferVarFS);

    // if we want to draw rectange, we need two triangles, but this case requires 6 vertices!
    // we can use an element buffer object (EBO) instead:
    // create  unique vertices first:
    float vertices[] = {
        0.5f, 0.5f, 0.0f, //top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f // top left
    };
    // then create indices:
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };


    td.prepareToDraw(vertices, indices);

    cb = [this]()
    {
        sp.use();
        td.execute();
    };
}

void ShadersExample::uniformColorExample()
{
    sp.createAndLink(transferVarVS, uniformVarFS);

    // if we want to draw rectange, we need two triangles, but this case requires 6 vertices!
    // we can use an element buffer object (EBO) instead:
    // create  unique vertices first:
    float vertices[] = {
        0.5f, 0.5f, 0.0f, //top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f // top left
    };
    // then create indices:
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };


    td.prepareToDraw(vertices, indices);


    cb = [this]()
    {
        // be sure to activate the shader:
        sp.use();

        // update the uniform color:
        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(sp.getId().value(), "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        // render:
        td.execute();
    };
}

void ShadersExample::operator()()
{
    if (cb)
    {
        cb();
    }
}
