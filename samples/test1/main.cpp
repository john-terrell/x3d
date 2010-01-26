#include <x3d/x3d.h>
#include <x3d/collection.h>
#include <x3d/auto_release_pool.h>
#include <x3d/opengl.h>
#include "window.h"

using namespace x3d;

void x3d_assertion_failed( const char *msg, const char *file, unsigned line )
{
    abort();
}


int main( int argc, char **argv )
{
    int status = 0;

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE );
 
    new window( 10, 10, 640, 480 );
    
    glutMainLoop();
    
    return status;
}
