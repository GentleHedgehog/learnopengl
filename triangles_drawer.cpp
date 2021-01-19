#include "triangles_drawer.h"
#include <cassert>

TrianglesDrawer::TrianglesDrawer()
{
    glGenVertexArrays(1, &VAO); // 1 - number of objects
    glGenBuffers(1, &VBO); // 1 - number of buffers
}

TrianglesDrawer::~TrianglesDrawer()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void TrianglesDrawer::execute()
{
    assert(isInit);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount); // 0 - start index, 3 - count of vertices
    glBindVertexArray(0);
}



TrianglesDrawerEBO::TrianglesDrawerEBO()
{
    glGenVertexArrays(1, &VAO); // 1 - number of objects
    glGenBuffers(1, &VBO); // 1 - number of buffers
    glGenBuffers(1, &EBO); // 1 - number of buffers
}

TrianglesDrawerEBO::~TrianglesDrawerEBO()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void TrianglesDrawerEBO::execute()
{
    assert(isInit);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,
                   elementsCount, // 6 vertices
                   GL_UNSIGNED_INT, //indices type
                   nullptr); // offset in EBO (or pass in index array(?), but not for EBO case)
    // glDrawElements takes indices from the EBO currently bound to GL_ELEMENT_ARRAY_BUFFER target
    // we can use VAO to automate EBO binding
    glBindVertexArray(0);
}


