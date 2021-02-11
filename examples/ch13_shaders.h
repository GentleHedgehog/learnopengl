#pragma once


/* basic lighting
 *
 *  
*/


#include <string>

const std::string basicLightingVS =
R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
//        layout (location = 1) in vec2 aTexCoord;

//        out vec2 TexCoord;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 proj;

        void main()
        {
            gl_Position = proj * view * model * vec4(aPos, 1.0f);
//            TexCoord = aTexCoord;
        }
)";

const std::string basicLightingFS =
R"(
        #version 330 core

        out vec4 FragColor;

        in vec2 TexCoord;

        //uniform sampler2D ourTexture1;
        //uniform sampler2D ourTexture2;
        uniform vec3 objectColor;
        uniform vec3 lightColor;

        void main()
        {
            //FragColor = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2);
            FragColor = vec4(objectColor * lightColor, 1.0);
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



