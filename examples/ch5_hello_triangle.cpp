#include "ch5_hello_triangle.h"
#include "shaders/ch5_simple_triangle.h"

HelloTriangle::HelloTriangle()
{    
    sp.createAndLink(simpleTriangleVertexShaderText, simpleTriangleFragmentShaderText);

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

//    TrianglesDrawer td;
//    td.prepareToDraw(vertices);


    td.prepareToDraw(vertices, indices);

//    auto func = [=, &sp, &td]()
//    {
//    };
}

void HelloTriangle::operator()()
{
    sp.use();
    td.execute();
}
