#include <iostream>
#include "opengl_framework.h"
#include "shaders/simple_triangle.h"
#include "shader_program.h"
#include "triangles_drawer.h"

using namespace std;

namespace  {

// 2d coord - precise value, but pixel is only approximation of that point (limited by resolution)

// graphics pipeline
/* vertex data[] -> (dot 3d coords (vertex: x, y, z, color, etc..)
 * vertex shader -> (vertex processing, change it)
 * shape assembly -> (assembles vertexes to primitives: lines, triangles, points, etc)
 * geometry shader -> (takes primitive and change it)
 * rasterization -> (map primitives to pixels: fragments, then clipping discards all fragments out of view,
 *                  fragment - all data required to render a single pixel)
 * fragment shader -> (calculate final color of pixel, consider lights, shadows, etc..)
 * alpha test and blending (checks depth, discard invisible, blends colors due to opacity)
 *
 * usually we change vertex and fragment shaders
 */

// normalized device coords - from -1 to 1 ---> NDC

}//nm

int main()
{
    OpenglFramework fw;

    std::cout << "start init" << std::endl;
    if (!fw.createWindowAsCurrentContext())
    {
        return -1;
    }

    ShaderProgram sp;

    std::cout << "start create vertex shader" << std::endl;
    sp.createVertexShader(simpleTriangleVertexShaderText.data());


    std::cout << "start create fragment shader" << std::endl;
    sp.createFragmentShader(simpleTriangleFragmentShaderText.data());

    std::cout << "start create program" << std::endl;
    sp.link();

    // if we want to draw rectange, we need two triangles, but this case requires 6 vertices!
    // we can use an element buffer object (EBO) instead:
    // create  unique vertices first:
    float vertices[] = {
        0.5f, 0.5f, 0.0f, //top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f // top left
    };
    // then create indices:
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };

//    TrianglesDrawer td;
//    td.prepareToDraw(vertices);

    TrianglesDrawerEBO td;
    td.prepareToDraw(vertices, indices);

    auto func = [=, &sp, &td]()
    {
        sp.use();
        td.execute();
    };

    std::cout << "start run" << std::endl;
    fw.run(func);

    return 0;
}
