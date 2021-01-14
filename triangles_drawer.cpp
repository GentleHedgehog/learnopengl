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
    glDrawArrays(GL_TRIANGLES, 0, 3); // 0 - start index, 3 - count of vertices
}


