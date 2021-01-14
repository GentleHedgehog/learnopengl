#include <iostream>
#include "opengl_framework.h"
#include "shaders/simple_triangle.h"

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

    auto checkShaderCompilation = [](unsigned int shaderId) -> bool {
        int success{};
        char infoLog[512];
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE)
        {
            glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
            std::cout << "compilation error for shader: id = " << shaderId << ", " << infoLog << std::endl;
        }
        else
        {
            std::cout << "shader compilation success, id = " << shaderId << std::endl;
        }
        return success == GL_TRUE;
    };


    std::cout << "start create vertex shader" << std::endl;
    unsigned int vertexShader{};
    {
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* data = simpleTriangleVertexShaderText.data();
        glShaderSource(vertexShader, 1/*count of strings*/,
                       &data, NULL);
        glCompileShader(vertexShader);
        if (! checkShaderCompilation(vertexShader))
        {
            return -1;
        }
    }

    std::cout << "start create fragment shader" << std::endl;
    unsigned int fragmentShader{};
    {
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* data = simpleTriangleFragmentShaderText.data();
        glShaderSource(fragmentShader, 1, &data, NULL);
        glCompileShader(fragmentShader);
        if (! checkShaderCompilation(fragmentShader))
        {
            return -1;
        }
    }

    std::cout << "start create program" << std::endl;
    unsigned int shaderProgram{};
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    {
        int success{};
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (success == GL_FALSE)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "link error for program: id = " << shaderProgram << ", " << infoLog << std::endl;
            return -1;
        }
        else
        {
            std::cout << "program link success" << std::endl;
        }
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    unsigned int VAO{}; // vertex array object - to save settings for dedicated vertices
    glGenVertexArrays(1, &VAO); // 1 - number of objects
    unsigned int VBO{};
    glGenBuffers(1, &VBO); // 1 - number of buffers

    glBindVertexArray(VAO); // start save settings for VBO

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // connect our vertex buffer to engine
    // copy data to connected buffer and it will not be changed:
    glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);
    // GL_DYNAMIC_DRAW - highly likely data will be changed
    // GL_STREAM_DRAW - data will be changed at each draw cycle


    // describe layout of vertices:
    // vertex attribute 0 will be associated with connected VBO:
    glVertexAttribPointer(0, // position vertex attribute: "layout (location = 0)" in shader
                          3, // vec3 -> 3 values
                          GL_FLOAT, // floating point values
                          GL_FALSE, // normalization flag (values was already normalized)
                          3 * sizeof (float), // stride (size of tightly packed vertex triple), can be 0 to define by OpenGL
                          (void*)0); // begin position offset of the data in buffer
    glEnableVertexAttribArray(0); //enable 0 vertex attrib (disabled by default)



    // unbind (stop save settings for VBO)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    auto func = [=]()
    {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3); // 0 - start index, 3 - count of vertices
    };

    std::cout << "start run" << std::endl;
    fw.run(func);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    return 0;
}
