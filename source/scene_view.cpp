#include <x3d/x3d.h>
#include <x3d/scene_view.h>
#include <x3d/reference.h>
#include <x3d/renderable.h>
#include <x3d/collection.h>
#include <x3d/iterator.h>
#include <x3d/auto_release_pool.h>
#include <x3d/opengl.h>

using namespace x3d;

struct x3d::scene_view_data
{
};

scene_view :: scene_view() : data( 0 )
{
    data = new scene_view_data;
}

scene_view :: ~scene_view()
{
    delete data;
    data = 0;
}

template < class T >
void perform_action( collection *a, void (T::*action)() )
{
    reference< iterator > i;
    a->get_iterator( &i );
    
    while( i->move_forward() )
    {
        reference< retainable > item;
        i->current( &item );
        
        T *action_interface = dynamic_cast< T * >( item.ref );
        if( action_interface )
            (action_interface->*action)();
        
        collection *col = 0;
        col = dynamic_cast< collection * >( item.ref );
        if( col )
            perform_action( col, action );
    }
}

void scene_view :: render( int width, int height )
{
    glClearColor( 0.4f, 0.4f, 0.4f, 1.0f );
    glColor4f( 0.9, 0.9, 0.9, 1.0f );
    
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_BLEND );
    glLineWidth( 1.0 );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    
    // fog
#if 0
    glEnable( GL_FOG );
    GLfloat fogColor[4] = { 0.5, 0.5, 0.5, 1.0 };
    glFogi( GL_FOG_MODE, GL_LINEAR );
    glFogfv( GL_FOG_COLOR, fogColor );
    glFogf( GL_FOG_DENSITY, 0.35 );
    glHint( GL_FOG_HINT, GL_NICEST );
    glFogf( GL_FOG_START, 4.5 );
    glFogf( GL_FOG_END, 7.0 );
#endif    
    
    glClear( GL_COLOR_BUFFER_BIT ); // | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glViewport( 0, 0, width, height );
    
    gluPerspective( 45, ( float )width / ( float )height, 0.1, 10000 );
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    gluLookAt( 4, 4, 4, 0, 2, 0, 0, 1, 0 );

    perform_action( this, &renderable::render );

    glLoadIdentity();

    gluLookAt( 4, 4, 4, 0, 2, 0, 0, 1, 0 );
    glScalef( 1, 0, 1 );
    glColor4f( 0.2, 0.2, 0.2, 1.0f );
    
    perform_action( this, &renderable::render );
}
