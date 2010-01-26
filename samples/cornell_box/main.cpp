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
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
 
    new window( 10, 10, 800, 600 );
    
    glutMainLoop();
    
    return status;
}
