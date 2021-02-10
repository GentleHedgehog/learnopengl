#include "ch12_colors.h"
#include "ch12_shaders.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <chrono>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "common_settings.h"
#include "image_framework.h"
#include "camera_system.h"

namespace {

void printMatrix4(const glm::mat4& mat, const std::string& name = "")
{
    std::cout << "matrix: " << name << std::endl;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << mat[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

}

Ch12_Colors::Ch12_Colors()
{
    createSceneWithLightSource();
}

void Ch12_Colors::createSceneWithLightSource()
{
    auto image = std::make_shared<ImageContainer>("container.jpg");
    auto image2 = std::make_shared<ImageContainer>("awesomeface.png", true);
    if (image->getData() && image2->getData())
    {
        texApplier.prepareData(image, 0);
        texApplier2.prepareData(image2, 1);

        // create the array of vertices attributes:
        float vertices[] = {
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
        };

        TrianglesDrawer::Attributes attr;
        attr.addAttribute(0, 3, 0);
        // we dont use EBO, because combination (vertex + texture coord) should be unique
        // and we cannot save much memory (one combination cannot be used by two facets due to diffrent tex coords)
        td.prepareToDraw(vertices, attr);


        spLighting.createAndLink(colorsVS, colorsLightFS);

        sp.createAndLink(colorsVS, colorsFS);
        sp.use();
        sp.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
        sp.setVec3("lightColor", glm::vec3(1.f, 1.f, 1.f));

        glEnable(GL_DEPTH_TEST);


        cb = [this](const nOpenglFramework::OpenglContextData& data)
        {
            static CameraSystem cs;
            cs.process(data.window);

            glClearColor(0.f, 0.f, 0.f, 1.0f); // black
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            texApplier.execute();
            texApplier2.execute();
            sp.use();

            glm::mat4 proj(1.0f);
            proj = glm::perspective(glm::radians(cs.getCurrentFOV()), CommonSettings::getAspectRatio(), 0.1f, 100.f);

            sp.setMat4("view", cs.getCurrentViewMatrix());
            sp.setMat4("proj", proj);

            glm::mat4 model(1.0f);
            sp.setMat4("model", model);

            td.execute();

            spLighting.use();
            glm::vec3 scaleForCube(0.2f, 0.2f, 0.2f);
            model = glm::scale(model, scaleForCube);
            glm::vec3 lightPos(3.2f, 1.0f, 5.0f);
            model = glm::translate(model, lightPos);
            spLighting.setMat4("model", model);
            spLighting.setMat4("view", cs.getCurrentViewMatrix());
            spLighting.setMat4("proj", proj);

            td.execute();
        };
    }
}


void Ch12_Colors::operator ()(const nOpenglFramework::OpenglContextData& data)
{
    if (cb)
    {
        cb(data);
    }
}
