#include <x3d/x3d.h>
#include <x3d/scene.h>
#include <x3d/scene_view.h>
#include <x3d/reference.h>
#include <x3d/auto_release_pool.h>
#include <x3d/iterator.h>

#include <x3d/opengl.h>

#include <map>

#include "window.h"
#include "radiosity.h"

using namespace x3d;

static std::map< int, window * > windowMap;

static bool shutdownCallbackRegistered = false;

struct window_data
{
    bool _needs_refresh;
    radiosity *_radiosity;
    bool _draw_wire_frame;
    bool _draw_elements;
    
    window_data() : _radiosity( 0 )
    {
    }
    
    ~window_data()
    {
        delete _radiosity;
    }
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

    int currentWindow = glutGetWindow();
    
    std::map< int, window * >::iterator i = windowMap.find( currentWindow );
    if( i != windowMap.end() )
        (*i).second->idle();
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

static void key_func( unsigned char key, int x, int y )
{
    int currentWindow = glutGetWindow();
    
    std::map< int, window * >::iterator i = windowMap.find( currentWindow );
    if( i != windowMap.end() )
        (*i).second->keyboard( key, x, y );
}

static void timer_func( int value )
{
    int currentWindow = glutGetWindow();
    
    std::map< int, window * >::iterator i = windowMap.find( currentWindow );
    if( i != windowMap.end() )
        (*i).second->timer( value );
}

static void create_sample_scene( window *wnd, scene *scn )
{
}

window :: window( int x, int y, unsigned width, unsigned height ) : 
    data( 0 ),
    _rotation_angle( 0 )
{
    data = new window_data;

    data->_needs_refresh = true;
    data->_draw_wire_frame = false;
    data->_draw_elements = false;
    
    if( !shutdownCallbackRegistered )
    {
        atexit( shutdownFunc );
        shutdownCallbackRegistered = true;
    }

    glutInitWindowPosition( x, y );
    glutInitWindowSize( width, height );
    
    glut_id = glutCreateWindow( "x3d Cornell Radiosity Box Test" );
    
    glutDisplayFunc( windowDisplayFunc );
    glutVisibilityFunc( windowVisibilityFunc );
    glutReshapeFunc( windowReshapeFunc );
    glutIdleFunc( windowIdleFunc );
    glutKeyboardFunc( key_func );
    glutTimerFunc( 10, timer_func, 23 );

    windowMap[ glut_id ] = this;
    
    data->_radiosity = new radiosity( glut_id, 512 );
    
    data->_radiosity->cornell_box();
    data->_radiosity->subdivide_patches( 6 );
    data->_radiosity->create_elements();
    data->_radiosity->subdivide_elements( 5 );
    data->_radiosity->initialize();
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


    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glColor3f( 1, 1, 1 );

   	glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );

    glEnable( GL_DEPTH_TEST );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glViewport( 0, 0, width, height );
    gluPerspective( 45, ( float )width / ( float )height, 100, 10000 );
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

#if 0
	glRotatef( 180, 0., 1., 0. );
	//glTranslatef( -278, -273, 800 );

	//glTranslatef( 278, 273, -800 );
	glRotatef( _rotation_angle, 0., 1., 0. );
	glTranslatef( -278, -273, 800 );
	
	glPushMatrix();
	glLoadIdentity();
#endif
	
    gluLookAt( 278, 273, -800, 278, 273, 0, 0, 1, 0 );
	
    if( data->_draw_wire_frame )
    {
        glPolygonMode( GL_FRONT, GL_LINE );
        glPolygonMode( GL_BACK, GL_LINE );
        glShadeModel( GL_FLAT );
        
        if( data->_draw_elements )
            data->_radiosity->draw_elements();
        else
            data->_radiosity->draw_patches();
    }
    else
    {
        glPolygonMode( GL_FRONT, GL_FILL );
        glPolygonMode( GL_BACK, GL_FILL );
        glShadeModel( GL_SMOOTH );
        data->_radiosity->draw_full();
    }

    glutSwapBuffers();
}

void window :: graph_node_change_triggered( graph_node *subject )
{
    glutPostRedisplay();
}

void window :: idle()
{
}

void window :: timer( int value )
{
    const float convergence = data->_radiosity->convergence();

    char window_title[ 256 ];
    if( convergence > 0.00001 )
    {
        data->_radiosity->execute_iteration();
        glutPostRedisplay();

        sprintf( window_title, "x3d Cornell Radiosity Box Test - (steps: %ld - %2.6f)", data->_radiosity->steps(), convergence );
        glutTimerFunc( 1, timer_func, 23 );
    }
    else
    {
        sprintf( window_title, "x3d Cornell Radiosity Box Test - (completed after steps: %ld)", data->_radiosity->steps() );
    }
    
    glutSetWindowTitle( window_title );
}

void window :: keyboard( unsigned char key, int x, int y )
{
    if( key == ' ' )
    {
        if( data->_draw_wire_frame )
            data->_draw_wire_frame = false;
        else
            data->_draw_wire_frame = true;
            
        glutPostRedisplay();
    }
    else
    if( key == 'd' )
    {
        data->_radiosity->execute_iteration();
        glutPostRedisplay();
    }
	else
	if( key == ',' )
	{
		_rotation_angle += 10;
	}
    else
	if( key == '.' )
	{
		_rotation_angle -= 10;
	}
	else
    if( key == 't' )
    {
        glutTimerFunc( 10, timer_func, 23 );
    }
    else
    if( key == 'p' )
    {
        if( data->_draw_elements )
            data->_draw_elements = false;
        else
            data->_draw_elements = true;
            
        glutPostRedisplay();
    }
}
