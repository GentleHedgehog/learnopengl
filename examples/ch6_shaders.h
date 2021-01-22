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

// ------- transfer vars:

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

// ------- uniform vars:


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


// ------- transfer vars:

// vertex1      vertex2
// xyz rgb      xyz rgb

std::string moreAttributesVS =
R"(
        #version 330 core
        layout (location = 0) in vec3 aPos; // attrib pos 0
        layout (location = 1) in vec3 aColor; // attrib pos 1
        out vec3 transColor;
        void main()
        {
            // vec3 temp = vec3(aPos.x + 0.5, aPos.yz); // to move to the right
            gl_Position = vec4(aPos, 1.0f); // upside down - (aPos * -1)
//            transColor = aPos; // transfer position as a color: negative value are clamped to 0 !
            transColor = aColor;
        }
)";


std::string moreAttributesFS =
R"(
        #version 330 core
        in vec3 transColor;
        out vec4 FragColor;
        void main()
        {
            FragColor = vec4(transColor, 1.0);
        }
)";
