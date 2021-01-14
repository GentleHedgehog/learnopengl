#pragma once

#include <glad/glad.h>
#include <iostream>


class TrianglesDrawer
{
public:
    TrianglesDrawer();
    ~TrianglesDrawer();

    template<int N>
    void prepareToDraw(const float (&vertices)[N])
    {
        glBindVertexArray(VAO); // start save settings for VBO

        glBindBuffer(GL_ARRAY_BUFFER, VBO); // connect our vertex buffer to engine
        // copy data to connected buffer and it will not be changed:
        glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);
        // GL_DYNAMIC_DRAW - highly likely data will be changed
        // GL_STREAM_DRAW - data will be changed at each draw cycle


        std::cout << "vertices sizeof = " << sizeof (vertices) << std::endl;

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

        isInit = true;
    }

    void execute();

private:
    bool isInit = false;
    unsigned int VAO{}; // vertex array object id - to save settings for dedicated vertices
    unsigned int VBO{}; // buffer with vertex data id
};

