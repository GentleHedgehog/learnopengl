#pragma once

#include <memory>
#include <functional>
#include <optional>

#include "shader_program.h"
#include "triangles_drawer.h"
#include "texture_applier.h"

// Part I, Chapters 8

class Ch8_Transformation
{
public:
    Ch8_Transformation();

    void operator() ();

private:
    ShaderProgram sp;
    TrianglesDrawerEBO tdEbo;
    TextureApplier texApplier;
    TextureApplier texApplier2;

    using CustomFunc = std::function<void()>;
    CustomFunc cb = nullptr;

    void putTextureOnRectangle();
    void putTwoTexturesOnRectangle();

};

