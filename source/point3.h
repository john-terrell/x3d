#pragma once

namespace x3d
{
    //
    // point3
    //
    struct point3
    {
        float _x, _y, _z;

    // construction
        point3()
        {
        }

        point3( float x, float y, float z ) :
            _x( x ),
            _y( y ),
            _z( z )
        {
        }

        bool is_valid() const
        {
            return( is_finite( _x ) && is_finite( _y ) && is_finite( _z ) );
        }
    };

    //
    // operator +
    //
    inline point3 operator + ( const point3 a, const point3 b )
    {
        return point3( a._x + b._x, a._y + b._y, a._z + b._z );
    }

    //
    // operator *
    //
    inline point3 operator * ( const point3 a, float b )
    {
        return point3( a._x * b, a._y * b, a._z * b );
    }
    
    //
    // operator /
    //
    inline point3 operator / ( const point3 &a, float b )
    {
        return point3( a._x / b, a._y / b, a._z / b );
    }
   
}
