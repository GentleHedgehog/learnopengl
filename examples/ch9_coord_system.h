#pragma once

#include <memory>
#include <functional>
#include <optional>

#include "shader_program.h"
#include "triangles_drawer.h"
#include "texture_applier.h"

// Part I, Chapters 9

class Ch9_CoordSystem
{
public:
    Ch9_CoordSystem();

    void operator() ();

private:
    ShaderProgram sp;
    TrianglesDrawerEBO tdEbo;
    TextureApplier texApplier;
    TextureApplier texApplier2;

    using CustomFunc = std::function<void()>;
    CustomFunc cb = nullptr;

    void putTwoTexturesOnRectangle();

};

