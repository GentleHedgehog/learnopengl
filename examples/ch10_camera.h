#pragma once

#include <memory>
#include <functional>
#include <optional>

#include "shader_program.h"
#include "triangles_drawer.h"
#include "texture_applier.h"

#include "common_definitions.h"

// Part I, Chapters 10

class Ch10_Camera
{
public:
    Ch10_Camera();

    void operator()(const nOpenglFramework::OpenglContextData& data);

private:
    ShaderProgram sp;
    TrianglesDrawerEBO tdEbo;
    TrianglesDrawer td;
    TextureApplier texApplier;
    TextureApplier texApplier2;

    nOpenglFramework::WorkerCallbackType cb = nullptr;

    void createCircleMovingAroundTheScene();
};

