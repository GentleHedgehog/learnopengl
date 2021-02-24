#pragma once

#include <memory>
#include <functional>
#include <optional>

#include "shader_program.h"
#include "triangles_drawer.h"
#include "texture_applier.h"

#include "common_definitions.h"

// Part II, Chapters 15

class Ch15_LightingMaps
{
public:
    Ch15_LightingMaps();

    void operator()(const nOpenglFramework::OpenglContextData& data);

private:
    ShaderProgram sp;
    ShaderProgram spLighting;
    TrianglesDrawerEBO tdEbo;
    TrianglesDrawer td;
//    TextureApplier t1;
//    TextureApplier t2;
    TextureVector texAppliers;

    nOpenglFramework::WorkerCallbackType cb = nullptr;

    void createSceneWithMaterial();
};

