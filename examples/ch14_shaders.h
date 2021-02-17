#pragma once


/* materials
 *
 *
*/


#include <string>

const std::string materialsVS =
R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 proj;

        out vec3 Normal;
        out vec3 FragPos; // in the world coord

        void main()
        {
            gl_Position = proj * view * model * vec4(aPos, 1.0f);
            Normal = mat3(transpose(inverse(model))) * aNormal;
            FragPos = vec3(model * vec4(aPos, 1.0));
        }
)";

const std::string materialsFS =
R"(
        #version 330 core

        out vec4 FragColor;

        uniform vec3 objectColor;
        uniform vec3 lightColor;

        in vec3 Normal;
        in vec3 FragPos;

        uniform vec3 lightPos;
        uniform vec3 viewPos;

        void main()
        {
            // AMBIENT:

            float ambientStrength = 0.1f;
            vec3 ambient = ambientStrength * lightColor;

            // DIFFUSE:

            vec3 norm = normalize(Normal); // why normalize a normal vector??
            // light's direction vector = light pos vector - fragment pos vector [LightPoint <--- FragmentPoint]
            vec3 lightDir = normalize(lightPos - FragPos); // normalize to make a work easier

            float diff = max(dot(norm, lightDir), 0.0); // prevent negative color value by max()
            vec3 diffuse = diff * lightColor;

            // SPECULAR:

            float specularStrength = 0.5f;
            vec3 viewDir = normalize(viewPos - FragPos);
            // first arg is vec from light source to fragment, second - normal vector for this fragment
            // -lightDir = [LightPoint ---> FragmentPoint]
            vec3 reflectDir = reflect(-lightDir, norm);

            int shininess = 32;
            float spec = pow(max(dot(viewDir, reflectDir), 0.f), shininess);
            vec3 specular = specularStrength * spec * lightColor;


            vec3 result = (specular + diffuse + ambient) * objectColor;
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



