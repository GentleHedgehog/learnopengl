#pragma once

#include <memory>
#include <functional>
#include <optional>

#include "shader_program.h"
#include "triangles_drawer.h"
#include "texture_applier.h"

// Part I, Chapters 10

class Ch10_Camera
{
public:
    Ch10_Camera();

    void operator() ();

private:
    ShaderProgram sp;
    TrianglesDrawerEBO tdEbo;
    TrianglesDrawer td;
    TextureApplier texApplier;
    TextureApplier texApplier2;

    using CustomFunc = std::function<void()>;
    CustomFunc cb = nullptr;

    void createCircleMovingAroundTheScene();
};

