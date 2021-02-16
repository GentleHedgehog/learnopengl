#pragma once


/* transforms
 *
 * vector: v = (x, y, z), k = (x2, y2, z2)
 *
 * vector len: len = sqrt(x^2 + y^2 + z^2)
 *
 * normalization: n = v / len = (x/len, y/len, z/len) ( n has len == 1)
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
 * each component == determinant == area of parallelogram (maid from two vectors)
 * v ~ pointing finger
 * k ~ middle finger
 * result ~ thumb  (RHS)
 * if we change: k x v -> LHS
 *
 * v x k = - (v x k)
 *
 * v x k = /v/ * /k/ * sin(teta)
 *
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
 * 1 0 0 0     x     x
 * 0 1 0 0  *  y  => y
 * 0 0 1 0     z     z
 * 0 0 0 1     1     1
 *
 * scaling:
 * uniform - if apply an equal factor for the each axe
 * s1 0 0 0     x * s1
 * 0 s2 0 0  => y * s2
 * 0 0 s3 0     z * s3
 * 0 0  0 1     1
 *
 * translation:
 * 1 0 0 Tx     x + Tx
 * 0 1 0 Ty  => y + Ty
 * 0 0 1 Tz     z + Tz
 * 0 0 0 1      1(w component - also known as homogeneous coordinate)
 *
 * w component:
 * - allow to do translation,
 * - used to get 3D coord as: (x/w, y/w, z/w)
 * - if equal to 0 - this is direction vector (cannot be translated)
 *
 * rotation: specified with an angle and a rotation axis
 * around X:            around Y:                   around Z:
 * 1 0    0   0          cos    0       sin 0        cos    -sin    0   0
 * 0 cos -sin 0          0      1       0   0        sin    cos     0   0
 * 0 sin  cos 0          -sin   0       cos 0        0      0       1   0
 * 0 0    0   1          0      0       0   1        0      0       0   1
 *
 * but a rotation around main axes introduces a problem - Gimbal lock
 * to minimize a probability of the problem - rotate around arbitrary unit axis ((0.662, 0.2, 0.722) for ex.)
 * but its rotation matrix has is more complex
 * sulution - use quaternions
 *
 *
 * combining:
 * to combine matrices we should multiply them
 * place matrices from the right to the left (in advised order):
 * [translation] * [rotation] * [scale]
 *
 *
 *
 *
*/


#include <string>

std::string transformationVS =
R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTexCoord;

        out vec2 TexCoord;

        uniform mat4 transform;

        void main()
        {
            gl_Position = transform * vec4(aPos, 1.0f);
            TexCoord = aTexCoord;
        }
)";

std::string transformationFS =
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


