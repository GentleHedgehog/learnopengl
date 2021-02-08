#include "ch10_camera.h"
#include "ch10_shaders.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <chrono>

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

struct CurrentEulerAngles
{
    static inline float pitch{};
    static inline float yaw{};
//    static inline bool isFirstCall = true;

    static glm::vec3 getNewFront()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        return front;
    }

    static void setNewPos(float x, float y)
    {
        static float lastX = x, lastY = y;

        float sensitivity = 0.05f; // to prevent sharp movements
        float xoffset = (x - lastX) * sensitivity;
        float yoffset = (lastY - y) * sensitivity;
        lastX = x;
        lastY = y;

        addOffset(xoffset, yoffset);
    }

    static void addOffset(float xoffset, float yoffset)
    {
        yaw  += xoffset;
        pitch += yoffset;

        if (pitch > 89.f)
        {
            pitch = 89.f;
        }
        else if (pitch < -89.f)
        {
            pitch = -89.f;
        }
    }
};

struct CurrentZoom
{
    static inline float fov = 45;
    static void setVerticalScrolling(double y)
    {
        if (fov >= 1.f && fov <= 45.f)
        {
            fov -= y;
        }
        else if (fov <= 1.f)
        {
            fov = 1.f;
        }
        else if (fov >= 45.f)
        {
            fov = 45.f;
        }

        std::cout << "new fow: " << fov << std::endl;
    }
};

struct CameraMovingCalculator
{
    float deltaTime = 0.f; // time between cur frame and last frame
    float lastFrame = 0.f; // time of last frame

    glm::vec3 cameraPos{0.f, 0.f, 3.f};
    glm::vec3 cameraFront{0.f, 0.f, -1.f};
    glm::vec3 cameraUp{0.f, 1.f, 0.f};

    bool isFirstCall = true;
    bool isCursorEnabled = true;

    glm::mat4 calculateLookAtMatrix()
    {
        cameraFront = glm::normalize(CurrentEulerAngles::getNewFront());

        return glm::lookAt(cameraPos,
        // ensures that camera keeps looking at the target direction (when we are moving):
                           cameraPos + cameraFront, // ~ target
                           cameraUp);
    }

    void processInput(GLFWwindow* w)
    {
        if (isFirstCall)
        {
            isFirstCall = false;
            // it should hide the cursor and capture it
            glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            isCursorEnabled = false;

            auto cbMove = [](GLFWwindow* w, double xpos, double ypos)
            {(void)w;
                CurrentEulerAngles::setNewPos(xpos, ypos);
            };
            glfwSetCursorPosCallback(w, cbMove);

            auto cbScroll = [](GLFWwindow* w, double x, double y)
            {(void)w; (void)x;
                // y - amount of scrolling vertically:
                CurrentZoom::setVerticalScrolling(y);
            };
            glfwSetScrollCallback(w, cbScroll);
        }


        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        float cameraSpeed = 2.5f * deltaTime;

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
        else if (glfwGetKey(w, GLFW_KEY_F2) == GLFW_PRESS)
        {
            using clock = std::chrono::steady_clock;
            static clock::time_point startDelay = clock::now();
            if ((clock::now() - startDelay) > std::chrono::seconds(1))
            {
                if (isCursorEnabled)
                {
                    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                }
                else
                {
                    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                }
                isCursorEnabled = !isCursorEnabled;
                startDelay = clock::now();
            }
        }
    }
};

}

Ch10_Camera::Ch10_Camera()
{
    createFreeMovingAroundTheScene();
}

void Ch10_Camera::createFreeMovingAroundTheScene()
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

            texApplier.execute();
            texApplier2.execute();
            sp.use();

            glm::mat4 proj(1.0f);
            proj = glm::perspective(glm::radians(CurrentZoom::fov), CommonSettings::getAspectRatio(), 0.1f, 100.f);

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
