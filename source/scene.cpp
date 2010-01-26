#include <x3d/x3d.h>
#include <x3d/scene.h>
#include <x3d/scene_view.h>
#include <x3d/reference.h>
#include <x3d/collection.h>
#include <x3d/auto_release_pool.h>
#include <list>

using namespace std;
using namespace x3d;

struct x3d::scene_data
{
    reference< collection > views;
    
    scene_data()
    {
        views.ref = new collection;
    }
};

scene :: scene() : 
    _data( 0 ),
    _modified( true ),
    _current_time( 0 ),
    _time_loop_minimum( 0 ),
    _time_loop_maximum( 3600 )
{
    _data = new scene_data;
}

scene :: ~scene() 
{
    delete _data;
    _data = 0;
}

void scene :: create_view( scene_view **new_view )
{
    assert( new_view );
    *new_view = 0;

    *new_view = new scene_view();

    (*new_view)->append( this );
}

void scene :: timer_callback()
{
    unsigned long current_time = _current_time.get_value();
    
    if( current_time + 1 > _time_loop_maximum.get_value() )
        _current_time.set_value( _time_loop_minimum.get_value() );
    else
        _current_time.set_value( ++current_time );
}
