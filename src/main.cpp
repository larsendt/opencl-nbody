#include "GLEngine.h"

int main(int argc, char** argv)
{
    GLEngine* ge = new GLEngine(argc, argv);
    int retval = ge->begin();
    delete ge;
    return retval;
}
