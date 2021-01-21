#pragma once

#include "shader_program.h"
#include "triangles_drawer.h"


class HelloTriangle
{
public:
    HelloTriangle();

    void operator() ();

private:
    ShaderProgram sp;
    TrianglesDrawerEBO td;
};

