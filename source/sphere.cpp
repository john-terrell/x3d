#include <x3d/x3d.h>
#include <x3d/sphere.h>
#include <x3d/opengl.h>

using namespace x3d;

sphere :: sphere()
{
}

void sphere :: render()
{
    glutWireSphere( 0.5f, 20, 20 );
}
