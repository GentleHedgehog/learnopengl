#pragma once

#include <memory>
#include <functional>
#include <optional>

#include "shader_program.h"
#include "triangles_drawer.h"

// Part I, Chapters 7

class Ch7_Textures
{
public:
    Ch7_Textures();

    void operator() ();

private:
    ShaderProgram sp;
    TrianglesDrawerEBO tdEbo;
    TrianglesDrawer td;

    using CustomFunc = std::function<void()>;
    CustomFunc cb = nullptr;

    void putTextureOnTriangle();
    void uniformColorExample();

    void moreAttributesExample();
};

