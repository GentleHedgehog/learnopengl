#pragma once

#include "shader_program.h"
#include "triangles_drawer.h"
#include <functional>

// Part I, Chapters 6

class ShadersExample
{
public:
    ShadersExample();

    void operator() ();

private:
    ShaderProgram sp;
    TrianglesDrawerEBO td;

    using CustomFunc = std::function<void()>;
    CustomFunc cb = nullptr;

    void transferColorExample();
    void uniformColorExample();

};

