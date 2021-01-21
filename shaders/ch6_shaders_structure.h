#pragma once


/* shaders structure
 *
 * #version version_number
 *
 * // limited by hardware (GL_MAX_VERTEX_ATTRIBS) number of attributes:
 * layout (location = 0) in type in_variable_name; // also known as a vertex attribute (4-component)
 * // the position vaar has attribute position 0
 * ..
 *
 * out type out_var_name;
 *
 * uniform type uniform_name;
 *
 * void main()
 * {
 *      // process inputs
 *      out_var_name = processed_stuff;
 * }
 *
 * -- types --
 * vecn: vector of n floats
 * bvecn: - booleans
 * ivecn: - integers
 * uvecn: - unsigned integers
 * dvecn: - doubles
 *
 * access to components:
 *  vec.x .y .z .w (or rgba, or stpq)
 *
 * swizzling:
 * vec2 someVec; // has only x, y
 * vec4 differentVec = someVec.xyxx; // has x, y, z, w
 * vec3 anotherVec = differentVec.zyw; // has x, y, z
 * vec4 otherVec = someVec.xxxx + anotherVec.yxzy
 *
 * substitution to ctor:
 * vec2 vect = vec2(0.5,0.7);
 * vec4 result = vec4(vec2, 0.0, 0.0);
 * vec4 otherResult = vec4(result.xyz, 1.0);
 *
 *
 * fragment shader requires output vec4 for color (if no -> black or white color)
 *
 * -- connect several shaders --
 *   vertex_shader:
 *      ..
 *      out vec4 vertexColor;
 *      ..
 *   fragment_shader:
 *      ..
 *      in vec4 vertexColor; // same name and type
 *      ..
 *
 * -- uniforms --
 * uniform is global (for program) variable:
 *   fragment_shader:
 *      ..
 *      uniform vec4 ourColor; // must be used somewhere in GLSL program (else it will be removed)
 *      ..
 *
 *
 *
 *
*/


#include <string>


std::string transferVarVS =
R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        out vec4 transColor;
        void main()
        {
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
            transColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
        }
)";


std::string transferVarFS =
R"(
        #version 330 core
        in vec4 transColor;
        out vec4 FragColor;
        void main()
        {
            FragColor = transColor;
        }
)";


std::string uniformVarFS =
R"(
        #version 330 core
        uniform vec4 ourColor;
        out vec4 FragColor;
        void main()
        {
            FragColor = ourColor;
        }
)";
