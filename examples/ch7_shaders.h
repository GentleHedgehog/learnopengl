#pragma once


/* textures
 *
 * coords (bottom left 0,0):
 * 0, 1     1, 1
 *
 *
 * 0, 0     1, 0
 *
 * we have to associate a vertex with texture coords:
 * pass coords to vertex shader, it pass them to fragment shader
 * fragment shader will interpolate all coords for each fragment
 *
 *
 * opengl will repeat texture if we specify coords outside of range 0..1:
 * GL_REPEAT - default
 * GL_MIRRORED_REPEAT - same, but mirrors image with each repeat
 * GL_CLAMP_TO_EDGE - clamp all coords to range 0..1 (stretch edges, edge pixels colors will be used)
 * GL_CLAMP_TO_BORDER - coords outside the range are given a user-specified border color (instead of edge pixel color)
 *
 * correspondence of coords:
 * vertex coord: x, y, z    -->   texture coords: s, t, r
 *
 * texture pixel ~ texel
 *
 * texture filtering options:
 * GL_NEAREST - opengl gets a color from a texel that is maximally near to the coord of the fragment (nearest neighbor filtering)
 * GL_LINEAR - takes an interpolated value from all neighboring texels (weighted sum) ((bi)linear filtering)
 *
 *
 * when texture are far away from observer (gets smaller), it becomes inefficient to work with high-resolution texture
 * to cope with this problem, opengl support 'mipmaps' - set of small copies of the texture (2x smaller on each step):
 * glGenerateMipmaps()
 *
 * filtering during switching between mipmap levels
 * (only for minifying filtering!!!, mipmap cannot be bigger than the original texture) :
 * GL_NEAREST_MIPMAP_NEAREST: first NEAREST - like GL_NEAREST, second NEAREST - to take mipmap level (based on pixel size (may be texel?))
 * GL_LINEAR_MIPMAP_NEAREST     -//-
 * GL_NEAREST_MIPMAP_LINEAR     -//-
 * GL_LINEAR_MIPMAP_LINEAR      -//-
 *
 *
 * We need to transfer texture to the fragment shader, so we have to use a built-in data type called 'sampler'
 *
*/


#include <string>

std::string textureVS =
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


std::string textureFS =
R"(
        #version 330 core

        out vec4 FragColor;

        in vec3 ourColor;
        in vec2 TexCoord;

        uniform sampler2D ourTexture;

        void main()
        {
            FragColor = texture(ourTexture, TexCoord); // texture func takes sampler and texture coordinate
        }
)";

std::string twoTexturesFS =
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


