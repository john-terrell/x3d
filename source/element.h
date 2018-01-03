#pragma once

#include "vector3.h"
#include "point3.h"
#include "color.h"

#include <vector>

struct vertex : public point3
{
    color _reflectance;
    unsigned _reflectance_count;
    unsigned _owning_patch;
    
    color reflectance()
    {
        const float recip = 1.0f / ( float )_reflectance_count;
        return color( _reflectance._red * recip, _reflectance._green * recip, _reflectance._blue * recip );
    }
    
    vertex( unsigned owning_patch = 0xFFFFFFFF, float x = 0, float y = 0, float z = 0 ) :
        point3( x, y, z ),
        _owning_patch( owning_patch ),
        _reflectance( 0, 0, 0 ),
        _reflectance_count( 0 )
    {
    }
    
    void reset_reflectance()
    {
        _reflectance = color( 0, 0, 0 );
        _reflectance_count = 0;
    }
    
    vertex & operator = ( const point3 & a )
    {
        _x = a._x;
        _y = a._y;
        _z = a._z;
        
        return *this;
    }
};

inline bool operator == ( const vertex &a, const vertex &b )
{
    if( a._owning_patch == b._owning_patch && a._x == b._x && a._y == b._y && a._z == b._z )
        return true;
        
    return false;
}

class element
{
    float _area;
    vector3	_normal;
    color _reflectance;  // in normal terms, surface color
    vertex *_vertices[ 4 ];
    unsigned _vertex_count;
    point3 _centroid;
    
public:
    element() :
        _vertex_count( 0 )
    {
        _vertices[ 0 ] = _vertices[ 1 ] = _vertices[ 2 ] = _vertices[ 3 ] = 0;
    }
    
    element( const color &reflectance, vertex *p1, vertex *p2, vertex *p3, vertex *p4 );
    
    float area() const;
    const vector3 &normal() const;
    const color &reflectance() const
    {
        return _reflectance;
    }
    
    const point3 &centroid() const;
    
    unsigned vertex_count() const
    {
        return _vertex_count;
    }
    
    vertex &vertex_number( unsigned index )
    {
        assert( index < _vertex_count );
        return *_vertices[ index ];
    }
    
    void calculate();
    
    unsigned subdivide( std::vector< element > &elements, std::vector< vertex > &vertices );
};
