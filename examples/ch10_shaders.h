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


