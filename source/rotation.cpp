#include <x3d/x3d.h>
#include <x3d/rotation.h>
#include <x3d/auto_release_pool.h>

using namespace x3d;

rotation :: rotation( float iangle, float idx, float idy, float idz ) :
    _angle( iangle ),
    _dx( idx ),
    _dy( idy ),
    _dz( idz )
{
    ensure_finite( idx, __FILE__, __LINE__ );
    ensure_finite( idy, __FILE__, __LINE__ );
    ensure_finite( idz, __FILE__, __LINE__ );
    
    watch_field_for_changes( &_angle );
    watch_field_for_changes( &_dx );
    watch_field_for_changes( &_dy );
    watch_field_for_changes( &_dz );
}

const matrix44 &rotation :: matrix()
{
    const float a = ( 3.14159265f / 180.0f ) * _angle.get_value();
    
    const float dx = _dx.get_value();
    const float dy = _dy.get_value();
    const float dz = _dz.get_value();
    
    const float c = ( float )cos( a );
    const float s = ( float )sin( a );
    const float u = 1.0f - ( float )cos( a );
    const float dx2 = dx * dx;
    const float dy2 = dy * dy;
    const float dz2 = dz * dz;
    
    _matrix._e[ 0 ][ 0 ] = ( dx2 * u ) + c;
    _matrix._e[ 0 ][ 1 ] = ( dx * dy * u ) - ( dz * s );
    _matrix._e[ 0 ][ 2 ] = ( dz * dx * u ) + ( dy * s );
    _matrix._e[ 0 ][ 3 ] = 0.0f;
    
    _matrix._e[ 1 ][ 0 ] = ( dx * dy * u ) + ( dz * s );
    _matrix._e[ 1 ][ 1 ] = ( dy2 * u ) + c;
    _matrix._e[ 1 ][ 2 ] = ( dz * dy * u ) - ( dx * s );
    _matrix._e[ 1 ][ 3 ] = 0.0f;
    
    _matrix._e[ 2 ][ 0 ] = ( dx * dz * u ) - ( dy * s );
    _matrix._e[ 2 ][ 1 ] = ( dy * dz * u ) + ( dx * s );
    _matrix._e[ 2 ][ 2 ] = ( dz2 * u ) + c;
    _matrix._e[ 2 ][ 3 ] = 0.0f;
    
    _matrix._e[ 3 ][ 0 ] = 
    _matrix._e[ 3 ][ 1 ] = 
    _matrix._e[ 3 ][ 2 ] = 0.0f;
    _matrix._e[ 3 ][ 3 ] = 1.0f;

    return transformation::matrix();
}

void rotation :: rotate( float angle_delta )
{
    _angle.set_value( _angle.get_value() + angle_delta );
}

