#pragma once

#include "shader_program.h"
#include "triangles_drawer.h"


class ShadersExample
{
public:
    ShadersExample();

    void operator() ();

private:
    ShaderProgram sp;
    TrianglesDrawerEBO td;
};

