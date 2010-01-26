#include <x3d/x3d.h>
#include <x3d/translation.h>
#include <x3d/auto_release_pool.h>

using namespace x3d;

translation :: translation( const vector3 &initial_offset ) :
    _translation( initial_offset )
{
    watch_field_for_changes( &_translation );
}

const matrix44 &translation :: matrix()
{
    _matrix.set_diagonal( 1.0f );

    const vector3 value = _translation.get_value(); 

    _matrix._e[ 3 ][ 0 ] = value._dx;
    _matrix._e[ 3 ][ 1 ] = value._dy;
    _matrix._e[ 3 ][ 2 ] = value._dz;

    return transformation::matrix();
}

void translation :: translate( const vector3 &offset )
{
    _translation.set_value( _translation.get_value() + offset );
}
