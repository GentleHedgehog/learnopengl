#include "ch6_shaders_examples.h"
#include "ch6_shaders.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

Ch6_ShadersExamples::Ch6_ShadersExamples()
{    
//    transferColorExample();
//    uniformColorExample();
    moreAttributesExample();
}

void Ch6_ShadersExamples::transferColorExample()
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


    tdEbo.prepareToDraw(vertices, indices);

    cb = [this]()
    {
        sp.use();
        tdEbo.execute();
    };
}

void Ch6_ShadersExamples::uniformColorExample()
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


    tdEbo.prepareToDraw(vertices, indices);


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
        tdEbo.execute();
    };
}

void Ch6_ShadersExamples::moreAttributesExample()
{
    sp.createAndLink(moreAttributesVS, moreAttributesFS);

    float vertices[] = {
        // positions        // colors
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f // top
    };

    TrianglesDrawer::Attributes attr;
    attr.addAttribute(0, 3, 0);
    attr.addAttribute(1, 3, 3);

    td.prepareToDraw(vertices, attr);

    cb = [this]()
    {
        sp.use();
        td.execute();
    };
}


void Ch6_ShadersExamples::operator()()
{
    if (cb)
    {
        cb();
    }
}
