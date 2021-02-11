#include <iostream>
#include "opengl_framework.h"
#include "common_settings.h"

//#include "examples/ch5_hello_triangle.h"
//#include "examples/ch6_shaders_examples.h"
//#include "examples/ch7_textures.h"
//#include "examples/ch8_transformation.h"
//#include "examples/ch9_coord_system.h"
//#include "examples/ch10_camera.h"
//#include "examples/ch12_colors.h"
#include "examples/ch13_basic_lighting.h"


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

    CommonSettings::width = 400;
    CommonSettings::height = 300;

    std::cout << "start init" << std::endl;
    if (!fw.createWindowAsCurrentContext(CommonSettings::width, CommonSettings::height))
    {
        return -1;
    }

    fw.printNumberOfAttributes();

//    Ch5_HelloTriangle example;
//    Ch6_ShadersExamples example;
//    Ch7_Textures example;
//    Ch8_Transformation example;
//    Ch9_CoordSystem example;
//    Ch10_Camera example;
//    Ch12_Colors example;
    Ch13_BasicLighting example;

    std::cout << "start run" << std::endl;
    fw.run(example);

    return 0;
}
