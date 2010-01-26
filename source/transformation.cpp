#include <x3d/x3d.h>
#include <x3d/transformation.h>
#include <x3d/auto_release_pool.h>
#include <x3d/opengl.h>

using namespace x3d;

transformation :: transformation()
{
}

void transformation :: render()
{
    const matrix44 &m = matrix();
    glMultMatrixf( &m._e[ 0 ][ 0 ] );
}
