#include <x3d/x3d.h>
#include <x3d/scene.h>
#include <x3d/scene_view.h>
#include <x3d/reference.h>
#include <x3d/auto_release_pool.h>
#include <x3d/iterator.h>

#include <x3d/sphere.h>
#include <x3d/cube.h>
#include <x3d/rotation.h>
#include <x3d/translation.h>
#include <x3d/opengl.h>

#include <map>

#include "window.h"

using namespace x3d;

static std::map< int, window * > windowMap;

static bool shutdownCallbackRegistered = false;

struct window_data
{
    bool _needs_refresh;
    reference< scene > scn;
    reference< scene_view > svw;
};

static void windowDisplayFunc()
{
    int currentWindow = glutGetWindow();
    
    std::map< int, window * >::iterator i = windowMap.find( currentWindow );
    if( i != windowMap.end() )
        (*i).second->refresh();
}

static void windowReshapeFunc( int, int )
{
    windowDisplayFunc();
}

static void windowIdleFunc()
{
    auto_release_pool::default_auto_release_pool->release_all_items();

    for( std::map< int, window * >::iterator i = windowMap.begin(); i != windowMap.end(); ++i )
    {
        float rotation = (*i).second->_rotation_angle.get_value();
        if( rotation + 1 < 360.0f )
            (*i).second->_rotation_angle.set_value( rotation + 1.0 );
        else
            (*i).second->_rotation_angle.set_value( 0 );
    }
}

static void windowVisibilityFunc( int state )
{
    if( state == GLUT_NOT_VISIBLE )
    {
    }
}

static void shutdownFunc()
{
    for( std::map< int, window * >::iterator i = windowMap.begin(); i != windowMap.end(); ++i )
    {
        delete (*i).second;
    }
    
    auto_release_pool::default_auto_release_pool->release_all_items();
}

static void create_sample_scene( window *wnd, scene *scn )
{
    retainable *r = 0;
    
    r = new rotation( 0, 0, 1, 0 );
    scn->append( r );
    
    field_connect( wnd->_rotation_angle, ((rotation*)r)->_angle ); 
    
    r->release();
    
    r = new translation( vector3( 0, 2, 0 ) );
    scn->append( r );
    r->release();
    
    r = new sphere;
    scn->append( r );
    r->release();
    
    r = new translation( vector3( 0, 0.25, 1.5 ) );
    scn->append( r );
    r->release();
    
    r = new cube;
    scn->append( r );
    r->release();

    r = new translation( vector3( 1.5, 0, -1.5 ) );
    scn->append( r );
    r->release();
    
    r = new sphere;
    scn->append( r );
    r->release();

    r = new translation( vector3( 0, 1, -3 ) );
    scn->append( r );
    r->release();

    r = new rotation( 45, 0, 1, 0 );
    scn->append( r );
    r->release();
   
    r = new cube;
    scn->append( r );
    r->release();
    
}

window :: window( int x, int y, unsigned width, unsigned height ) : 
    data( 0 ),
    _rotation_angle( 0 )
{
    data = new window_data;

    data->_needs_refresh = true;
    data->scn.ref = new scene;
    
    create_sample_scene( this, data->scn );
    
    data->scn->create_view( &data->svw );

    if( !shutdownCallbackRegistered )
    {
        atexit( shutdownFunc );
        shutdownCallbackRegistered = true;
    }

    data->svw->add_change_observer( *this );

    glutInitWindowPosition( x, y );
    glutInitWindowSize( width, height );
    
    glut_id = glutCreateWindow( "x3d Test #1" );
    
    glutDisplayFunc( windowDisplayFunc );
    glutVisibilityFunc( windowVisibilityFunc );
    glutReshapeFunc( windowReshapeFunc );
    glutIdleFunc( windowIdleFunc );

    windowMap[ glut_id ] = this;
}

window :: ~window()
{
    delete data;
    data = 0;
}

void window :: refresh()
{
    const int width = glutGet( GLUT_WINDOW_WIDTH );
    const int height = glutGet( GLUT_WINDOW_HEIGHT );
    
    data->svw->render( width, height );

    glutSwapBuffers();
}

void window :: graph_node_change_triggered( graph_node *subject )
{
    glutPostRedisplay();
}

