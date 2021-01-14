#include <iostream>
#include "opengl_framework.h"

using namespace std;

namespace  {

}//nm

int main()
{
    OpenglFramework fw;

    if (!fw.initialize())
    {
        return -1;
    }

    return 0;
}
