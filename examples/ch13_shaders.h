#pragma once


/* basic lighting
 *
 *  Phong lighting model - approximation of reality (for colors), using simplified models
 *  it is combination of the main lighting components:
 *  - ambient - always gives the object some color (there are some sources of light: the moon, distant light, etc.)
 *  - diffuse - simulate directional impact of light (the more an object face the light source, the brighter it is)
 *  - specular - simulate the bright spot on shiny objects (inclined to the color of the light)
 *
 *  ambient - is a simple analog of global illumination (complex model of light reflection from objects)
 *
 *
*/


#include <string>

const std::string basicLightingVS =
R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 proj;

        void main()
        {
            gl_Position = proj * view * model * vec4(aPos, 1.0f);
        }
)";

const std::string basicLightingFS =
R"(
        #version 330 core

        out vec4 FragColor;

        uniform vec3 objectColor;
        uniform vec3 lightColor;

        void main()
        {
            float ambientStrength = 0.1f;
            vec3 ambient = ambientStrength * lightColor;

            vec3 result = ambient * objectColor;
            FragColor = vec4(result, 1.0);
        }
)";

const std::string lampFS =
R"(
        #version 330 core

        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(1.0);
        }
)";



