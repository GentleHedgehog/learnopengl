#pragma once


/* transforms
 *
 * vector: v = (x, y, z), k = (x2, y2, z2)
 *
 * vector len: len = sqrt(x^2 + y^2 + z^2)
 *
 * normalization: n = v / len ( n has len == 1)
 *
 * dot product: v * k = /v/ * /k/ * cos(teta) = x * x2 + y * y2 + z * z2
 *
 *
 * cross product:
 * is only definded in 3D space, takes two non-parallel vectors,
 * produces third vector that is orthogonal to input vectors
 *
 * v x k = ((y * z2 - z * y2), (z * x2 - x * z2), (x * y2 - y * x2))
 *
 * matrices:
 *
 *      scalar: (x, y) op 2 = (x op 2, y op 2) [where op: +, -, *, : ]
 *      matrix: (x, y) op (x2, y2) = (x op x2, y op y2) [op: +, -]
 *
 * matrix multiplying:
 * A * B = C,
 * A must have a number of columns equal to the number of rows in B
 * the result element of C will be equal to the dot product of the row of A with the column of B
 *
 *
 * identity matrix: does not change vector
 *
 *
*/


#include <string>

std::string transformationVS =
R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        layout (location = 2) in vec2 aTexCoord;

        out vec3 ourColor;
        out vec2 TexCoord;

        void main()
        {
            gl_Position = vec4(aPos, 1.0f);
            ourColor = aColor;
            TexCoord = aTexCoord;
        }
)";

std::string transformationFS =
R"(
        #version 330 core

        out vec4 FragColor;

        in vec3 ourColor;
        in vec2 TexCoord;

        uniform sampler2D ourTexture1;
        uniform sampler2D ourTexture2;

        void main()
        {
            // mix gets two colors and blend them
            // (factor: 0 - return first color, 1.0 - second color, 0.2 - 80% first 20% second ??)
            FragColor = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2);
        }
)";


