#pragma once

#include <memory>
#include <functional>
#include <optional>

#include "shader_program.h"
#include "triangles_drawer.h"
#include "texture_applier.h"

#include "common_definitions.h"

// Part II, Chapters 14

class Ch14_Materials
{
public:
    Ch14_Materials();

    void operator()(const nOpenglFramework::OpenglContextData& data);

private:
    ShaderProgram sp;
    ShaderProgram spLighting;
    TrianglesDrawerEBO tdEbo;
    TrianglesDrawer td;
    TextureApplier texApplier;
    TextureApplier texApplier2;

    nOpenglFramework::WorkerCallbackType cb = nullptr;

    void createSceneWithLightSource();
};

