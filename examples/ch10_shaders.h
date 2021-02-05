#pragma once


/* camera
 * 
 * camera has its own coor system
 *
 *
 * 'look at' matrix:
 *
 * rotate      *  translate
 *
 * Rx Ry Rz 0     1 0 0 -Px
 * Ux Uy Uz 0  *  0 1 0 -Py
 * Dx Dy Dz 0     0 0 1 -Pz
 * 0  0  0  1     0 0 0 1
 *
 * Camera space traits:
 * R - right vector
 * U - up
 * D - direction
 *
 * P - point of the camera in the world (negative due to: we want translate the world in the opposite direction)
 *
 *
 * deltaTime - useful measure to calculate an equal camera speed (for slow and fast hardware)
 *
 * // let's get the direction vector:

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
*/


#include <string>

std::string cameraVS =
R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTexCoord;

        out vec2 TexCoord;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 proj;

        void main()
        {
            gl_Position = proj * view * model * vec4(aPos, 1.0f);
            TexCoord = aTexCoord;
        }
)";

std::string cameraFS =
R"(
        #version 330 core

        out vec4 FragColor;

        in vec2 TexCoord;

        uniform sampler2D ourTexture1;
        uniform sampler2D ourTexture2;

        void main()
        {
            // mix gets two colors and blend them
            FragColor = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2);
        }
)";


