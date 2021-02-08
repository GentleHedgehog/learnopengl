#include "camera_system.h"

#include <iostream>
#include <chrono>


namespace {

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
