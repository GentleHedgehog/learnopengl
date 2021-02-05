#include "ch10_camera.h"
#include "ch10_shaders.h"

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

struct CameraMovingCalculator
{
    glm::vec3 cameraPos{0.f, 0.f, 3.f};
    glm::vec3 cameraFront{0.f, 0.f, -1.f};
    glm::vec3 cameraUp{0.f, 1.f, 0.f};

    glm::mat4 calculateLookAtMatrix()
    {
        return glm::lookAt(cameraPos,
        // ensures that camera keeps looking at the target direction (when we are moving):
                           cameraPos + cameraFront, // ~ target
                           cameraUp);
    }

    void processInput(GLFWwindow* w)
    {
        float cameraSpeed = 0.05f;
        if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS)
        {
            cameraPos += cameraSpeed * cameraFront;
        }
        else if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS)
        {
            cameraPos -= cameraSpeed * cameraFront;
        }
        else if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS)
        {
            cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
        }
        else if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS)
        {
            cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
        }
    }
};

}

Ch10_Camera::Ch10_Camera()
{    
    // let's get the direction vector:

    // +z is going through your screen towards you (move camera backward):
    auto cameraPos = glm::vec3(0.f, 0.f, 3.f);

    // see at the world center:
    auto cameraTarget = glm::vec3(0.f, 0.f, 0.f);

    // direction vector points from the target to the camera (!):
    auto cameraDirection = glm::normalize(cameraPos - cameraTarget); // normalize because we need only direction
    // v1 - v2 = v3 (points from v2 to v1)


    // let's get the right vector (+x in camera space (RHS)):

    // little trick - get 'up' vector and use cross-product to get +x vector:
    auto up = glm::vec3(0.f, 1.f, 0.f); // up in the world space
    auto cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    // thanks to Gram-Shmidt process in linear algebra

    // do the same to get a proper up vector of the camera:
    auto cameraUp = glm::cross(cameraDirection, cameraRight);


    auto view = glm::lookAt(cameraPos, cameraTarget, up);

    // to rotate around the scen around the circle
    float radius = 10.f;
    float camX = sin(glfwGetTime()) * radius;
    float camZ = cos(glfwGetTime()) * radius;


    createCircleMovingAroundTheScene();
}

void Ch10_Camera::createCircleMovingAroundTheScene()
{
    auto image = std::make_shared<ImageContainer>("container.jpg");
    auto image2 = std::make_shared<ImageContainer>("awesomeface.png", true);
    if (image->getData() && image2->getData())
    {
        texApplier.prepareData(image, 0);
        texApplier2.prepareData(image2, 1);

        // create the array of vertices attributes:
        float vertices[] = {
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


        sp.createAndLink(cameraVS, cameraFS);

        sp.use();
        // set uniform values once:
        sp.setInt("ourTexture1", 0);
        sp.setInt("ourTexture2", 1);

        glEnable(GL_DEPTH_TEST);


        cb = [this, cubePositions](const nOpenglFramework::OpenglContextData& data)
        {
            // for free moving with WSAD keys on keyboard:
            static CameraMovingCalculator movingCalc;

            movingCalc.processInput(data.window);

            glClear(GL_DEPTH_BUFFER_BIT);

//            static float deltaTime = 0.f; // time between cur frame and last frame
//            static float lastFrame = 0.f; // time of last frame
//            float currentFrame = glfwGetTime();
//            deltaTime = currentFrame - lastFrame;
//            lastFrame = currentFrame;
//            float cameraSpeed = 2.5f * deltaTime;

            texApplier.execute();
            texApplier2.execute();
            sp.use();

            glm::mat4 proj(1.0f);
            auto fov = glm::radians(45.f);
            proj = glm::perspective(fov, CommonSettings::getAspectRatio(), 0.1f, 100.f);

            sp.setMat4("view", movingCalc.calculateLookAtMatrix());
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


void Ch10_Camera::operator ()(const nOpenglFramework::OpenglContextData& data)
{
    if (cb)
    {
        cb(data);
    }
}
