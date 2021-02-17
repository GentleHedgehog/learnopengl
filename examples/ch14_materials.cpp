#include "ch14_materials.h"
#include "ch14_shaders.h"

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

Ch14_Materials::Ch14_Materials()
{
    createSceneWithLightSource();
}

void Ch14_Materials::createSceneWithLightSource()
{
    auto image = std::make_shared<ImageContainer>("container.jpg");
    auto image2 = std::make_shared<ImageContainer>("awesomeface.png", true);
    if (image->getData() && image2->getData())
    {
        texApplier.prepareData(image, 0);
        texApplier2.prepareData(image2, 1);

        // create the array of vertices attributes:
        float vertices[] = {
            // coords             // normales
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };

        TrianglesDrawer::Attributes attr;
        attr.addAttribute(0, 3, 0);
        attr.addAttribute(1, 3, 3);
        // we dont use EBO, because combination (vertex + texture coord) should be unique
        // and we cannot save much memory (one combination cannot be used by two facets due to diffrent tex coords)
        td.prepareToDraw(vertices, attr);


        spLighting.createAndLink(materialsVS, lampFS);

        sp.createAndLink(materialsVS, materialsFS);
        sp.use();
        sp.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
        sp.setVec3("lightColor", glm::vec3(1.f, 1.f, 1.f));

        glEnable(GL_DEPTH_TEST);

        static const glm::vec3 cameraPos{0.f, 0.f, 3.f};

        cb = [this](const nOpenglFramework::OpenglContextData& data)
        {
            static const glm::vec3 scaleForCube(0.2f, 0.2f, 0.2f);
            static glm::vec3 lightPos(3.2f, 1.0f, 5.0f);

            auto radius = 10;
            lightPos.x = sin((glfwGetTime())) * radius;
            lightPos.z = cos((glfwGetTime())) * radius;


            static CameraSystem cs(cameraPos);
            cs.process(data.window);

            glClearColor(0.0f, 0.2f, 0.f, 1.0f); // dark green
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
            sp.setVec3("lightPos", lightPos);
            sp.setVec3("viewPos", cs.getCurrentPosition());

            td.execute();

            spLighting.use();            
            model = glm::scale(model, scaleForCube);            
            model = glm::translate(model, lightPos);
            spLighting.setMat4("model", model);
            spLighting.setMat4("view", cs.getCurrentViewMatrix());
            spLighting.setMat4("proj", proj);

            td.execute();
        };
    }
}


void Ch14_Materials::operator ()(const nOpenglFramework::OpenglContextData& data)
{
    if (cb)
    {
        cb(data);
    }
}
