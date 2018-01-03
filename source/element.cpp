#include "x3d.h"
#include "element.h"

element :: element( const color &reflectance, vertex *p1, vertex *p2, vertex *p3, vertex *p4 ) : _reflectance( reflectance )
{
    assert( p1 );
    assert( p2 );
    assert( p3 );
    
    // it's valid for p4 to be null when the client wants to create a triangle
    _vertices[ 0 ] = p1;
    _vertices[ 1 ] = p2;
    _vertices[ 2 ] = p3;
    
    if( p4 )
    {
        _vertices[ 3 ] = p4;
        _vertex_count = 4;
    }
    else
    {
        _vertices[ 3 ] = 0;
        _vertex_count = 3;
    }
        
    calculate();
}

float element :: area() const
{
    return _area;
}

const vector3 & element :: normal() const
{
    return _normal;
}

const point3 & element :: centroid() const
{
    return _centroid;
}

void element :: calculate()
{
    const vector3 va = *_vertices[ 0 ] - *_vertices[ 1 ];
    const vector3 vb = *_vertices[ 0 ] - *_vertices[ 2 ];
    
    _normal = cross_product( va, vb );
    
    const float normal_length = _normal.length();
    _area = normal_length * 0.5f;
    
    _normal *= 1.0f / normal_length;
    
    if( _vertex_count > 3 )
    {
        const vector3 vc = *_vertices[ 3 ] - *_vertices[ 0 ];
        
        _area += cross_product( vb, vc ).length() * 0.5f;
        _centroid = *_vertices[ 0 ] + ( ( *_vertices[ 2 ] - *_vertices[ 0 ] ) * 0.5f );
    }
    else
    {
        _centroid = ( *_vertices[ 0 ] + *_vertices[ 1 ] + *_vertices[ 2 ] ) / 3;
    }
}

void element :: subdivide( std::vector< element > &elements, std::vector< vertex > &vertices )
{
    elements.push_back( *this );
    element &r = elements[ elements.size() - 1 ];

    if( _vertex_count == 4 )
    {
        r._vertex_count = 3;
        r._vertices[ 0 ] = _vertices[ 2 ];
        r._vertices[ 1 ] = _vertices[ 3 ];
        r._vertices[ 2 ] = _vertices[ 0 ];
        
        _vertex_count = 3;
        _vertices[ 3 ] = 0;
    }
    else
    {
        const vector3 e1 = *_vertices[ 1 ] - *_vertices[ 0 ];
        const vector3 e2 = *_vertices[ 2 ] - *_vertices[ 1 ];
        const vector3 e3 = *_vertices[ 0 ] - *_vertices[ 2 ];
        
        const float e1_sl = e1.squared_length();
        const float e2_sl = e2.squared_length();
        const float e3_sl = e3.squared_length();
        
        vertex new_point( _vertices[ 0 ]->_owning_patch, 0, 0, 0 );
        if( e1_sl > e2_sl && e1_sl > e3_sl )
        {
			
			
            new_point = ( *_vertices[ 0 ] + *_vertices[ 1 ] ) / 2.0f;
			std::vector< vertex >::iterator i = std::find( vertices.begin(), vertices.end(), new_point );
            if( i == vertices.end() )
            {
                vertices.push_back( new_point );
                r._vertices[ 0 ] = &vertices[ vertices.size() - 1 ];
                r._vertices[ 1 ] = _vertices[ 1 ];
                r._vertices[ 2 ] = _vertices[ 2 ];
                _vertices[ 1 ] = &vertices[ vertices.size() - 1 ];
            }
            else
            {
                vertex *p = &(*i);
                r._vertices[ 0 ] = p;
                r._vertices[ 1 ] = _vertices[ 1 ];
                r._vertices[ 2 ] = _vertices[ 2 ];
                _vertices[ 1 ] = p;
            }
        }
        else
        if( e2_sl > e1_sl && e2_sl > e3_sl )
        {
            new_point = ( *_vertices[ 2 ] + *_vertices[ 1 ] ) / 2.0f;
            std::vector< vertex >::iterator i = find( vertices.begin(), vertices.end(), new_point );
            if( i == vertices.end() )
            {
                vertices.push_back( new_point );
                r._vertices[ 0 ] = _vertices[ 0 ];
                r._vertices[ 1 ] = &vertices[ vertices.size() - 1 ];
                r._vertices[ 2 ] = _vertices[ 2 ];
                _vertices[ 2 ] = &vertices[ vertices.size() - 1 ];
            }
            else
            {
                vertex *p = &(*i);
                r._vertices[ 0 ] = _vertices[ 0 ];
                r._vertices[ 1 ] = p;
                r._vertices[ 2 ] = _vertices[ 2 ];
                _vertices[ 2 ] = p;
            }
        }
        else
        {
            new_point = ( *_vertices[ 0 ] + *_vertices[ 2 ] ) / 2.0f;
            std::vector< vertex >::iterator i = find( vertices.begin(), vertices.end(), new_point );
            if( i == vertices.end() )
            {
                vertices.push_back( new_point );
                r._vertices[ 0 ] = _vertices[ 0 ];
                r._vertices[ 1 ] = _vertices[ 1 ];
                r._vertices[ 2 ] = &vertices[ vertices.size() - 1 ];
                _vertices[ 0 ] = &vertices[ vertices.size() - 1 ];
            }
            else
            {
                vertex *p = &(*i);
                r._vertices[ 0 ] = _vertices[ 0 ];
                r._vertices[ 1 ] = _vertices[ 1 ];
                r._vertices[ 2 ] = p;
                _vertices[ 0 ] = p;
            }
        }
        
        r._vertex_count = 3;
    }

    calculate();
    r.calculate();
}
