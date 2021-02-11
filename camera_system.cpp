#include "camera_system.h"

#include <iostream>
#include <chrono>
#include "common_settings.h"

namespace {

struct CurrentEulerAngles
{
    static inline float pitch{};
    static inline float yaw = -90; // to look at the center
//    static inline bool isFirstCall = true;

    static glm::vec3 getNewFront()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);

        return front;
    }

    static void setNewPos(float x, float y)
    {
        static float lastX{}, lastY{};

//        if (isFirstCall)
//        {
//            lastX = CommonSettings::width / 2;
//            lastY = CommonSettings::height / 2;

//            if (x > CommonSettings::width)
//                lastX = x;
//            if (y > CommonSettings::height)
//                lastY = y;

//            isFirstCall = false;
//        }

//        std::cout << "lastX " << lastX << std::endl;
//        std::cout << "lastY " << lastY << std::endl;
//        std::cout << "x " << x << std::endl;
//        std::cout << "y " << y << std::endl;

        float sensitivity = 0.05f; // to prevent sharp movements
        float xoffset = (x - lastX) * sensitivity;
        float yoffset = (lastY - y) * sensitivity;
        lastX = x;
        lastY = y;

//        std::cout << "xoffset " << xoffset << std::endl;
//        std::cout << "yoffset " << yoffset << std::endl;


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
    glm::vec3 worldUp{0.f, 1.f, 0.f};

    bool isCursorEnabled = true;
    bool isFirstCall = true;

    glm::mat4 calculateLookAtMatrix()
    {        
        cameraFront = CurrentEulerAngles::getNewFront();

        auto cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
        auto cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));

//        std::cout << "pos: x " << cameraPos.x << " y " << cameraPos.y << " z " << cameraPos.z << std::endl;
//        std::cout << "front: x " << cameraFront.x << " y " << cameraFront.y << " z " << cameraFront.z << std::endl;
//        std::cout << "up: x " << cameraUp.x << " y " << cameraUp.y << " z " << cameraUp.z << std::endl;

        return glm::lookAt(cameraPos,
        // ensures that camera keeps looking at the target direction (when we are moving):
                           cameraPos + cameraFront, // ~ target
                           cameraUp);
    }

    void processInput(GLFWwindow* w)
    {
        if (isFirstCall)
        {
            init(w);
            isFirstCall = false;
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
            cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, worldUp));
        }
        else if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS)
        {
            cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, worldUp));
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

    void init(GLFWwindow* w)
    {
        // it should hide the cursor and capture it
        glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPos(w, CommonSettings::width/2, CommonSettings::height/2);
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
};

// for free moving with WSAD keys on keyboard:
CameraMovingCalculator movingCalc;

} // nm

CameraSystem::CameraSystem()
{
}

void CameraSystem::process(GLFWwindow* w)
{
    movingCalc.processInput(w);
}

float CameraSystem::getCurrentFOV()
{
    return CurrentZoom::fov;
}

glm::mat4 CameraSystem::getCurrentViewMatrix()
{
    return movingCalc.calculateLookAtMatrix();
}
