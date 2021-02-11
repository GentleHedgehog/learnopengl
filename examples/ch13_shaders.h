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
 *      can be calculated as an object color multiplied by a small part of the light source
 *
 *  diffuse - based on the angle of incidence of the light beam (check the angle between a light beam and a normal vector)
 *      the more angle the fewer value of the color
 *      the angle can be calculated by dot product (of normalized vectors)
 *
 *      for working with normal vectors in world coordinates we should use 'normal matrix':
 *          - normal matrix is 'the transponse of the inverse of the upper-left corner of the model matrix'
 *          - normal matrix allow to apply model matrix to normal vectors without distortions
 *              distortions might be arise from non-uniform scaling (inside model matrix)
 *          - we dont need a translation for normal vectors (it cannot change their direction, and their positions dont matter)
 *             so we need only 3x3 matrix for scaling and rotation
 *
 *      inversing matrix is a costly operation - avoid to do this operation inside a shader!
 *
 *  specular - light reflected from a fragment in the opposite direction (with an equal angle from a normal)
 *      we see this fragment from view direction
 *      the closer the angle between reflection and view vector, the greater the impact of the specular light
 *      view vector can be calculated as diff = view point - fragment point (in world coords)
 *          it can be done in a view space too, where view point is always at (0, 0, 0), but it is not so intuitive
 *
*/


#include <string>

const std::string basicLightingVS =
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

const std::string basicLightingFS =
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



