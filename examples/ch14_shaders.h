#pragma once


/* materials
 *
 *  real objects reflect light relative to their material
 *
 *  ambient, diffuse, specular components and shininess make a full material trait structure
 *
 *  to restrict the light influence we create structure for light properties (position, phong's components):
 *  each component defines an intensity
 *  ambient - set it to low intensity to decrease its domination
 *  diffuse - set to the exact color we would like a light to have (often white)
 *  specular - is usually kept at vec3(1.0) shining at full intensity
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

        struct Material{
            vec3 ambient; // object color under ambient lighting
            vec3 diffuse; // object color under diffuse lighting
            vec3 specular;  // color impact a specular light has on the object (object specific specular highlight color)
            float shininess; // scattering/radius of the specular highlight
        };

        uniform Material material;


        struct Light{
            vec3 position;
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
        };

        uniform Light light;

        out vec4 FragColor;

        uniform vec3 objectColor;
        uniform vec3 lightColor;

        in vec3 Normal;
        in vec3 FragPos;

        uniform vec3 viewPos;

        void main()
        {
            // AMBIENT:
            vec3 ambient = material.ambient * light.ambient;

            // DIFFUSE:

            vec3 norm = normalize(Normal); // why normalize a normal vector??
            // light's direction vector = light pos vector - fragment pos vector [LightPoint <--- FragmentPoint]
            vec3 lightDir = normalize(light.position - FragPos); // normalize to make a work easier

            float diff = max(dot(norm, lightDir), 0.0); // prevent negative color value by max()
            vec3 diffuse = (diff * material.diffuse) * light.diffuse;

            // SPECULAR:
            vec3 viewDir = normalize(viewPos - FragPos);
            // first arg is vec from light source to fragment, second - normal vector for this fragment
            // -lightDir = [LightPoint ---> FragmentPoint]
            vec3 reflectDir = reflect(-lightDir, norm);

            float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
            vec3 specular = (spec * material.specular) * light.specular;

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



