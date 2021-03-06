#pragma once

#include "shader_program.h"
#include "triangles_drawer.h"
#include <functional>

// Part I, Chapters 6

class Ch6_ShadersExamples
{
public:
    Ch6_ShadersExamples();

    void operator() ();

private:
    ShaderProgram sp;
    TrianglesDrawerEBO tdEbo;
    TrianglesDrawer td;

    using CustomFunc = std::function<void()>;
    CustomFunc cb = nullptr;

    void transferColorExample();
    void uniformColorExample();

    void moreAttributesExample();
};

