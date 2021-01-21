#include <iostream>
#include "opengl_framework.h"
#include "examples/hello_triangle.h"

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

    fw.printNumberOfAttributes();

    HelloTriangle ht;

    std::cout << "start run" << std::endl;
    fw.run(ht);

    return 0;
}
