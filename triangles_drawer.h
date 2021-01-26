#pragma once

#include <glad/glad.h>
#include <iostream>
#include <vector>


class TrianglesDrawer
{
public:
    TrianglesDrawer();
    ~TrianglesDrawer();

    struct AttributeInfo
    {
        GLuint locationPosition{};
        GLint countOfElements{};
        size_t offsetInElements{};

        void print() const
        {
            std::cout << "attr: loc " << locationPosition
                      << " count " << countOfElements
                      << " offset " << offsetInElements << std::endl;
        }
    };

    struct Attributes
    {
        GLsizei stride{};
        void addAttribute(GLuint locationPosition, GLint countOfElements, size_t offsetInElements)
        {
            AttributeInfo newAttr{locationPosition, countOfElements, offsetInElements};
            attrs.push_back(newAttr);
            stride += countOfElements;
        }
        std::vector<AttributeInfo> attrs;

        void print() const
        {
            std::cout << "attributes: stride " << stride << std::endl;
            for(const auto& a: attrs)
            {
                a.print();
            }
        }
    };


    template<int N>
    void prepareToDraw(const float (&vertices)[N])
    {
        Attributes attr;
        attr.addAttribute(0, 3, 0);
        prepareToDraw(vertices, attr);
    }

    template<int N>
    void prepareToDraw(const float (&vertices)[N], const Attributes& attrs)
    {
        glBindVertexArray(VAO); // start save settings for VBO

        attrs.print();
        loadVbo(VBO, vertices, attrs);

        glBindVertexArray(0); // unbind vao

        vertexCount = N / attrs.stride;
        std::cout << "vertex count: " << vertexCount << std::endl;
        isInit = true;
    }

    void execute();

    template<int N>
    static void loadVbo(unsigned int VBO, const float (&vertices)[N], const Attributes& attrs)
    {
        std::cout << "load vbo -- vertices sizeof = " << sizeof (vertices) << std::endl;

        glBindBuffer(GL_ARRAY_BUFFER, VBO); // connect our vertex buffer to engine
        // copy data to connected buffer and it will not be changed:
        glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);
        // GL_DYNAMIC_DRAW - highly likely data will be changed
        // GL_STREAM_DRAW - data will be changed at each draw cycle

        applyAttributes(attrs);

        // unbind (stop save settings for VBO)
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    static void applyAttributes(const Attributes& attrs)
    {
        for (const auto& attribute : attrs.attrs)
        {
            // describe layout of vertices:
            // vertex attribute 0 will be associated with connected VBO:
            glVertexAttribPointer(attribute.locationPosition, // position vertex attribute: "layout (location = 0)" in shader
                                  attribute.countOfElements, // vec3 -> 3 values
                                  GL_FLOAT, // floating point values
                                  GL_FALSE, // normalization flag (values was already normalized)
                                  attrs.stride * sizeof (float), // stride (size of tightly packed vertex triple), can be 0 to define by OpenGL
                                  (void*)(attribute.offsetInElements * sizeof (float))); // begin position offset of the data in buffer
            glEnableVertexAttribArray(attribute.locationPosition); //enable 0 vertex attrib (disabled by default)
        }
    }

private:
    bool isInit = false;
    int vertexCount = 0;
    unsigned int VAO{}; // vertex array object id - to save settings for dedicated vertices
    unsigned int VBO{}; // buffer with vertex data id
};

class TrianglesDrawerEBO
{
public:
    TrianglesDrawerEBO();
    ~TrianglesDrawerEBO();

    template<int N, int M>
    void prepareToDraw(const float (&vertices)[N], const unsigned int (&indices)[M])
    {
        TrianglesDrawer::Attributes attr;
        attr.addAttribute(0, 3, 0);
        prepareToDraw(vertices, indices, attr);
    }

    template<int N, int M>
    void prepareToDraw(const float (&vertices)[N],
                       const unsigned int (&indices)[M],
                       const TrianglesDrawer::Attributes& attrs)
    {
        glBindVertexArray(VAO); // start save settings for VBO

        TrianglesDrawer::loadVbo(VBO, vertices, attrs);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // unbind EBO: remember - DO NOT unbind EBO while a VAO is active !!!! VAO does it itself
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // unbind VAO:
        glBindVertexArray(0);

        elementsCount = M;

        isInit = true;

        std::cout << "vertices sizeof = " << sizeof (vertices) << std::endl;
        std::cout << "indices sizeof = " << sizeof (indices) << std::endl;
        std::cout << VAO << ", " << VBO << ", " << EBO << ", " << elementsCount << std::endl;

        bool isFillTrianglesWithColor = true;
        if (isFillTrianglesWithColor)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
    }

    void execute();

private:
    bool isInit = false;
    GLsizei elementsCount = 0;
    unsigned int VAO{}; // vertex array object id - to save settings for dedicated vertices
    unsigned int VBO{}; // buffer with vertex data id
    unsigned int EBO{}; // buffer with vertex indices
};

