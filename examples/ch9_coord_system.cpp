#include "ch9_coord_system.h"
#include "ch9_shaders.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "common_settings.h"
#include "image_framework.h"

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

Ch9_CoordSystem::Ch9_CoordSystem()
{    
//    createPlaneIn3DWorld();
    createCubeIn3DWorld();
}

void Ch9_CoordSystem::createPlaneIn3DWorld()
{
    auto image = std::make_shared<ImageContainer>("container.jpg");
    auto image2 = std::make_shared<ImageContainer>("awesomeface.png", true);
    if (image->getData() && image2->getData())
    {
        texApplier.prepareData(image, 0);
        texApplier2.prepareData(image2, 1);

        // create the array of vertices attributes:
        float vertices[] = {
            // positions xyz     // texture coords st
            0.5f, 0.5f, 0.0f,     1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f,    1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f,    0.0f, 1.0f // top left
        };

        unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3 // second triangle
        };

        TrianglesDrawer::Attributes attr;
        attr.addAttribute(0, 3, 0);
        attr.addAttribute(1, 2, 3);

        tdEbo.prepareToDraw(vertices, indices, attr);

        sp.createAndLink(coordSystemVS, coordSystemFS);

        sp.use();
        // set uniform values once:
        sp.setInt("ourTexture1", 0);
        sp.setInt("ourTexture2", 1);


        cb = [this]()
        {
            texApplier.execute();
            texApplier2.execute();
            sp.use();            

            glm::mat4 model(1.0f);
            auto rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f);
            // our plane is slightly on the floor:
            model = glm::rotate(model, glm::radians(-55.f), rotationAxis);

            glm::mat4 view(1.0f);
            // translate along -z (imitate the camera backward moving in RHS):
            view = glm::translate(view, glm::vec3(0.f, 0.f, -3.f));

            glm::mat4 proj(1.0f);
            auto fov = glm::radians(45.f);
            proj = glm::perspective(fov, CommonSettings::getAspectRatio(), 0.1f, 100.f);


            unsigned int modelLoc = glGetUniformLocation(sp.getId().value(), "model");
            glUniformMatrix4fv(modelLoc,
                               1,  // how many matrices
                               GL_FALSE, // transpose flag (to get column-major ordering), glm has already such ordering
                               glm::value_ptr(model)); // proper matrix representation

            unsigned int viewLoc = glGetUniformLocation(sp.getId().value(), "view");
            glUniformMatrix4fv(viewLoc,
                               1,  // how many matrices
                               GL_FALSE, // transpose flag (to get column-major ordering), glm has already such ordering
                               glm::value_ptr(view)); // proper matrix representation

            unsigned int projLoc = glGetUniformLocation(sp.getId().value(), "proj");
            glUniformMatrix4fv(projLoc,
                               1,  // how many matrices
                               GL_FALSE, // transpose flag (to get column-major ordering), glm has already such ordering
                               glm::value_ptr(proj)); // proper matrix representation

            tdEbo.execute();
        };
    }
}

void Ch9_CoordSystem::createCubeIn3DWorld()
{
    auto image = std::make_shared<ImageContainer>("container.jpg");
    auto image2 = std::make_shared<ImageContainer>("awesomeface.png", true);
    if (image->getData() && image2->getData())
    {
        texApplier.prepareData(image, 0);
        texApplier2.prepareData(image2, 1);

        // create the array of vertices attributes:
        float vertices[] = {
             // vertex pos        // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };

        TrianglesDrawer::Attributes attr;
        attr.addAttribute(0, 3, 0);
        attr.addAttribute(1, 2, 3);

        // we dont use EBO, because combination (vertex + texture coord) should be unique
        // and we cannot save much memory (one combination cannot be used by two facets due to diffrent tex coords)
        td.prepareToDraw(vertices, attr);


        glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
        };


        sp.createAndLink(coordSystemVS, coordSystemFS);

        sp.use();
        // set uniform values once:
        sp.setInt("ourTexture1", 0);
        sp.setInt("ourTexture2", 1);

        glEnable(GL_DEPTH_TEST);

        cb = [this, cubePositions]()
        {
            glClear(GL_DEPTH_BUFFER_BIT);

            texApplier.execute();
            texApplier2.execute();
            sp.use();

            glm::mat4 view(1.0f);
            // translate along -z (imitate the camera backward moving in RHS):
            view = glm::translate(view, glm::vec3(0.f, 0.f, -3.f));

            glm::mat4 proj(1.0f);
            auto fov = glm::radians(45.f);
            proj = glm::perspective(fov, CommonSettings::getAspectRatio(), 0.1f, 100.f);

            sp.setMat4("view", view);
            sp.setMat4("proj", proj);

            for (unsigned int i = 0; i < 10; i++)
            {
                glm::mat4 model(1.0f);
                model = glm::translate(model, cubePositions[i]);
                auto rotationAxis = glm::vec3(1.0f, 0.3f, 0.5f);
//                float currentAngle = 20.f * i;
                auto currentAngle = static_cast<float>(glfwGetTime()) * glm::radians(50.f);
                model = glm::rotate(model, currentAngle, rotationAxis);
                sp.setMat4("model", model);

                td.execute();
            }
        };
    }
}


void Ch9_CoordSystem::operator()()
{
    if (cb)
    {
        cb();
    }
}
