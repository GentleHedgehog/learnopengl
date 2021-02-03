#pragma once


/* coordinate system (CS)
 *
 * work in a specific CS may be easier
 *
 * 5 CSs:
 * - local/object space - with local origin
 * - world space - with global origin
 * - view/eye/camera space - as seen from the camera
 * - clip space - process the coords to -1.0...1.0 and determine the visible vertices
 * - screen space - transform coords from -1.0...1.0 to a viewport coord range (800x600 for ex.)
 *
 *  local -> [MODEL_MATRIX] -> world -> [VIEW_MATRIX] -> view -> [PROJECTION_MATRIX] -> clip -> VIEWPORT_TRANSFORM -> screen
 *
 *  take a model, put it to the world, view on it and project it on the surface of the camera glass
 *
 *  local:
 *      all vertices are in the local space
 *      for container -> -0.5, 0.5, 0.0 -> local coords
 *
 *  world:
 *      model matrix scales, rotates, translates objects to the world places
 *
 *  view:
 *      view matrix rotates and translates the scene to the front of the camera
 *
 *  clip:
 *      projection matrix transform coords to NDC (-1000...1000 to -1.0...1.0 for example)
 *      not visible vertices are clipped (outside the range: -1000...1000 for example)
 *      if a part of a triangle is clipped, OpenGL will reconstruct ti as one or more triangles
 *
 *      viewing box - is called 'frustum', all vertices in frustum will be visible on the screen
 *
 *      final operation - perspective division (x/w, y/w, z/w) (it is an automatic op after each vertex shader run)
 *
 *      projection matrix types:
 *      - orthographic projection
 *          box frustum (near, far plane, height, width), w component is untouched (always == 1)
 *          glm::ortho(0 - left_coord, 800 - right, 0 - bottom, 600 - top, near - 0.1, far - 100);
 *      - perspective projection
 *          the further away a vertex the higher its w component becomes
 *          coords transform to range -w...w
 *          glm::perspective(fov, aspect_ratio, near, far)
 *              fov - field of view (usually 45deg)
 *              aspect_ration - width/height
 *
 *
 *  V_clip = M_proj * M_view * M_model * V_local
 *
 *  mapping to a viewport range - VIEWPORT_TRANSFORM
 *
 * Right-handed system (RHS, by default in OpenGL):
 *      center of the screen ~ (0, 0, 0) coord
 *      +Y is up
 *      +X is to the right
 *      +Z is going through the screen towards you
 *      thumb(+x), pointing finger(+y), middle finger(+z, towards you)
 *
 *  left-handed system (LHS) is used in DirectX and -> _after projection matrix_ (in NDC) !!!
 *      thumb(+x), pointing finger(+y), middle finger(+z, away from you)
 *
 *
 * OpenGL gives no guarantee on the order of triangles rendered (within the same draw call)
 *  we have to use the z-buffer (depth-testing) to properly render the overlapping pixels!
 *  glwf creates depth buffer for you and save depth value in the z-coord components of each fragment
 *  we have to enable the depth testing:
 *      glEnable(GL_DEPTH_TEST);
 *      ...
 *      // clear the bit before rendering in the game cycle. That's all we should do for the depth testing!
 *      glClear(GL_DEPTH_BUFFER_BIT);
 *
 *
*/


#include <string>

std::string coordSystemVS =
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

std::string coordSystemFS =
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


