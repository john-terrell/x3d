#include <x3d/x3d.h>
#include <x3d/cube.h>
#include <x3d/opengl.h>

using namespace x3d;

cube :: cube()
{
}

void cube :: render()
{
    glutWireCube( 1.0f );
}