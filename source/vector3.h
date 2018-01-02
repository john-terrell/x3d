#pragma once

#include "point3.h"

namespace x3d
{
    //
    // vector3
    //
    struct vector3
    {
        float _dx, _dy, _dz;

    // construction
        vector3() :
            _dx( NAN ),
            _dy( NAN ),
            _dz( NAN )
        {
        }

        vector3( float dx, float dy, float dz ) :
            _dx( dx ),
            _dy( dy ),
            _dz( dz )
        {
            assert( is_valid() );
        }

    // instance methods
        bool is_valid() const
        {
            return ( std::isfinite( _dx ) && std::isfinite( _dy ) && std::isfinite( _dz ) ? true : false );
        }

        //
        // squared_length()
        //
        float squared_length() const
        {
            assert( is_valid() );
            return ( _dx * _dx ) + ( _dy * _dy ) + ( _dz * _dz );
        }

        //
        // length()
        //
        float length() const
        {
            return static_cast< float >( ::sqrt( squared_length() ) );
        }

        //
        // set_length()
        //
        void set_length( float new_length )
        {
            assert( std::isfinite( new_length ) );
            assert( is_valid() );
            const float scale = new_length / length();

            assert( std::isfinite( scale ) );

            _dx *= scale;
            _dy *= scale;
            _dz *= scale;

            assert( is_valid() );
        }

        //
        // reflection() - calculates the reflection of this vector about the given normal
        //
        vector3 reflect( const vector3& n )
        {
            assert( n.is_valid() );
            assert( is_valid() );
            assert( 0 );

            return vector3();
        }

        void operator += ( const vector3& a )
        {
            _dx += a._dx;
            _dy += a._dy;
            _dz += a._dz;
        }
        
        void operator *= ( float a )
        {
            assert( std::isfinite( a ) );
        
            _dx *= a;
            _dy *= a;
            _dz *= a;
        }
        
        bool operator == ( const vector3 &a ) const
        {
            return ( _dx == a._dx && _dy == a._dy && _dz == a._dz );
        }

        bool operator != ( const vector3 &a ) const
        {
            return ( _dx != a._dx || _dy != a._dy || _dz != a._dz );
        }
    };

    //
    // dot_product()
    //
    inline float dot_product( const vector3& a, const vector3& b )
    {
        assert( a.is_valid() && b.is_valid() );
        return ( a._dx * b._dx ) + ( a._dy * b._dy ) + ( a._dz * b._dz );
    }

    //
    // cross_product()
    //
    inline vector3 cross_product( const vector3& a, const vector3& b )
    {
        assert( a.is_valid() && b.is_valid() );
#if 0
        return vector3(
            ( a._dy * b._dz ) - ( a._dz * b._dy ),
            ( a._dz * b._dx ) - ( a._dx * b._dz ),
            ( a._dx * b._dy ) - ( a._dy * b._dx )
            );
#else
        return vector3(
            -(( a._dy * b._dz ) - ( a._dz * b._dy )),
            -(( a._dz * b._dx ) - ( a._dx * b._dz )),
            -(( a._dx * b._dy ) - ( a._dy * b._dx ))
            );
#endif
    }

    inline vector3 operator * ( const vector3& a, float b )
    {
        assert( a.is_valid() && std::isfinite( b ) );
        return vector3( a._dx * b, a._dy * b, a._dz * b );
    }

    inline point3 operator + ( const point3& a, const vector3& b )
    {
        assert( a.is_valid() && b.is_valid() );
        return point3( a._x + b._dx, a._y + b._dy, a._z + b._dz );
    }

    inline vector3 operator + ( const vector3 &a, const vector3 &b )
    {
        assert( a.is_valid() && b.is_valid() );
        return vector3( a._dx + b._dx, a._dy + b._dy, a._dz + b._dz );
    }

    inline vector3 operator - ( const vector3& a )
    {
        return vector3( -a._dx, -a._dy, -a._dz );
    }

    inline vector3 operator - ( const point3& a, const point3& b )
    {
        return vector3( a._x - b._x, a._y - b._y, a._z - b._z );
    }

    inline vector3 operator - ( const vector3& a, const vector3& b )
    {
        return vector3( a._dx - b._dx, a._dy - b._dy, a._dz - b._dz );
    }

    inline vector3 operator / ( const vector3& a, float s )
    {
        return vector3( a._dx / s, a._dy / s, a._dz / s );
    }
    
    inline float rand_neg1_to_1()
    {
        const float frand_max = ( float )RAND_MAX;
        return ( 2 * ( ( float )rand() / frand_max ) ) - 1.0f;
    }
    
    inline vector3 random_vector()
    {
        vector3 r( rand_neg1_to_1(), rand_neg1_to_1(), rand_neg1_to_1() );
        
        r.set_length( 1.0f );
        
        return r;
    }

} // namespace x3d
