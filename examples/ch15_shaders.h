#pragma once


/* lighting maps
 *
 * To colour different parts of an object we need 'diffuse map'
 *
 * diffuse map is a texture
 * we replace a diffuse color to a sampler2D inside Material struct
 *
 * Notice! sampler2D is a 'opaque type' (in OpenGL), we can instantiate it as a uniform variable only!
 *
 * Ambient color is in almost all cases equal to the diffuse color,
 * but if you want to use it, use another texture for ambient values
 *
 * Using tools like Photoshop or Gimp it is relatively easy to transform a diffuse texture
 * to a specular image like this by cutting out some parts,
 * transforming it to black and white and increasing the brightness/contrast.
 *
*/


#include <string>

const std::string lightingMapsVS =
R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        layout (location = 2) in vec2 aTexCoords;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 proj;

        out vec3 Normal;
        out vec3 FragPos; // in the world coord
        out vec2 TexCoords;

        void main()
        {
            gl_Position = proj * view * model * vec4(aPos, 1.0f);
            Normal = mat3(transpose(inverse(model))) * aNormal;
            FragPos = vec3(model * vec4(aPos, 1.0));
            TexCoords = aTexCoords;
        }
)";

const std::string lightingMapsFS =
R"(
        #version 330 core

        struct Material{
            sampler2D diffuse;
            sampler2D specular;
            float shininess;
        };

        uniform Material material;

        in vec2 TexCoords;

        struct Light{
            vec3 position;
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
        };

        uniform Light light;

        out vec4 FragColor;

        uniform vec3 objectColor;

        in vec3 Normal;
        in vec3 FragPos;

        uniform vec3 viewPos;

        void main()
        {
            vec3 texelDiffuse = vec3(texture(material.diffuse, TexCoords));
            vec3 texelSpecular = texture(material.specular, TexCoords).rgb;

            // AMBIENT (equal to diffuse):
            vec3 ambient = texelDiffuse * light.ambient;

            // DIFFUSE (from texture):

            vec3 norm = normalize(Normal); // why normalize a normal vector??
            // light's direction vector = light pos vector - fragment pos vector [LightPoint <--- FragmentPoint]
            vec3 lightDir = normalize(light.position - FragPos); // normalize to make a work easier

            float diff = max(dot(norm, lightDir), 0.0); // prevent negative color value by max()
            vec3 diffuse = (diff * texelDiffuse) * light.diffuse;

            // SPECULAR:
            vec3 viewDir = normalize(viewPos - FragPos);
            // first arg is vec from light source to fragment, second - normal vector for this fragment
            // -lightDir = [LightPoint ---> FragmentPoint]
            vec3 reflectDir = reflect(-lightDir, norm);

            float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
            vec3 specular = (spec * texelSpecular) * light.specular;

            FragColor = vec4(specular + diffuse + ambient, 1.0);
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



