#include <iostream>
#include "opengl_framework.h"
#include "shaders/simple_triangle.h"
#include "shader_program.h"
#include "triangles_drawer.h"

using namespace std;

namespace  {

}//nm

int main()
{
    OpenglFramework fw;

    std::cout << "start init" << std::endl;
    if (!fw.createWindowAsCurrentContext())
    {
        return -1;
    }

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


    ShaderProgram sp;

    std::cout << "start create vertex shader" << std::endl;
    sp.createVertexShader(simpleTriangleVertexShaderText.data());


    std::cout << "start create fragment shader" << std::endl;
    sp.createFragmentShader(simpleTriangleFragmentShaderText.data());

    std::cout << "start create program" << std::endl;
    sp.link();



    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    std::cout << "vertices sizeof origin = " << sizeof (vertices) << std::endl;

    TrianglesDrawer td;
    td.prepareToDraw(vertices);

    auto func = [=, &sp, &td]()
    {
        sp.use();
        td.execute();
    };

    std::cout << "start run" << std::endl;
    fw.run(func);

    return 0;
}
