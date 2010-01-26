#ifndef x3d__matrix44_h
#define x3d__matrix44_h

#ifndef x3d__point3_h
#include <x3d/point3.h>
#endif

#ifndef x3d__vector3_h
#include <x3d/vector3.h>
#endif

#include <stdexcept>
#include <algorithm>	// for swap()
#include <math.h>

namespace x3d
{
    //
    // utility functions
    //
    inline float determinant_2x2(
        float a, float b, float c, float d )

    {
        assert( is_finite( a ) );
        assert( is_finite( b ) );
        assert( is_finite( c ) );
        assert( is_finite( d ) );

        return a * d - b * c;
    }

    inline float determinant_3x3( 
        float a1, float a2, float a3,
        float b1, float b2, float b3,
        float c1, float c2, float c3 )
    {
        assert( is_finite( a1 ) );
        assert( is_finite( a2 ) );
        assert( is_finite( a3 ) );
        assert( is_finite( b1 ) );
        assert( is_finite( b2 ) );
        assert( is_finite( b3 ) );
        assert( is_finite( c1 ) );
        assert( is_finite( c2 ) );
        assert( is_finite( c3 ) );

        return a1 * determinant_2x2( b2, b3, c2, c3 ) -
               b1 * determinant_2x2( a2, a3, c2, c3 ) +
               c1 * determinant_2x2( a2, a3, b2, b3 );
    }

	//
	// singular_matrix_error
	//
	class singular_matrix_error : public std::runtime_error
	{
	public:
		singular_matrix_error() : std::runtime_error( "singular matrix inversion attempted" )
		{
		}
	};
	
    //
    // matrix44
    //
    struct matrix44
    {
        float   _e[ 4 ][ 4 ];

    // construction
        matrix44()
        {
        }

        matrix44( float diagonal )
        {
            set_diagonal( diagonal );
        }

    // operators
        const float&  operator ()( unsigned short r, unsigned short c ) const
        {
            return _e[ r ][ c ];
        }

        float&  operator ()( unsigned short r, unsigned short c )
        {
            return _e[ r ][ c ];
        }

    // instance methods
        bool is_valid() const
        {
            const float sum = 
                _e[ 0 ][ 0 ] +
                _e[ 0 ][ 1 ] +
                _e[ 0 ][ 2 ] +
                _e[ 0 ][ 3 ] +
                _e[ 1 ][ 0 ] +
                _e[ 1 ][ 1 ] +
                _e[ 1 ][ 2 ] +
                _e[ 1 ][ 3 ] +
                _e[ 2 ][ 0 ] +
                _e[ 2 ][ 1 ] +
                _e[ 2 ][ 2 ] +
                _e[ 2 ][ 3 ] +
                _e[ 3 ][ 0 ] +
                _e[ 3 ][ 1 ] +
                _e[ 3 ][ 2 ] +
                _e[ 3 ][ 3 ];

            return is_finite( sum );
        }

        //
        // set_diagonal()
        //
        void set_diagonal( float diagonal )
        {
            assert( is_finite( diagonal ) );

            _e[ 0 ][ 1 ] =
            _e[ 0 ][ 2 ] =
            _e[ 0 ][ 3 ] =
            _e[ 1 ][ 0 ] =
            _e[ 1 ][ 2 ] =
            _e[ 1 ][ 3 ] =
            _e[ 2 ][ 0 ] =
            _e[ 2 ][ 1 ] =
            _e[ 2 ][ 3 ] =
            _e[ 3 ][ 0 ] =
            _e[ 3 ][ 1 ] =
            _e[ 3 ][ 2 ] = 0.0f;

            _e[ 0 ][ 0 ] =
            _e[ 1 ][ 1 ] =
            _e[ 2 ][ 2 ] =
            _e[ 3 ][ 3 ] = diagonal;
        }
                
        //
        // perspective()
        //
        void perspective( float view_distance )
        {
            assert( is_finite( view_distance ) );

        	set_diagonal( 1.0f );
        	
        	_e[ 3 ][ 3 ] = 0.0;
        	_e[ 2 ][ 3 ] = 1.0f / view_distance;
        }

		//
		// find_transpose()
		//
		matrix44 find_transpose() const
		{
            assert( is_valid() );

            matrix44 xpose;

			for( short r = 0; r < 4; ++r )
				for( short c = 0; c < 4; ++c )
					xpose._e[ r ][ c ] = _e[ c ][ r ];

            return xpose;
		}
		        
        //
        // find_inverse()
        //
        matrix44 find_inverse() const
        {
            assert( is_valid() );

            matrix44 result;

            const float det = determinant();

            if( fabs( det ) < 0.000001 )
                throw singular_matrix_error();
        
            const float& a1 = _e[ 0 ][ 0 ];
            const float& b1 = _e[ 0 ][ 1 ];
            const float& c1 = _e[ 0 ][ 2 ];
            const float& d1 = _e[ 0 ][ 3 ];

            const float& a2 = _e[ 1 ][ 0 ];
            const float& b2 = _e[ 1 ][ 1 ];
            const float& c2 = _e[ 1 ][ 2 ];
            const float& d2 = _e[ 1 ][ 3 ];

            const float& a3 = _e[ 2 ][ 0 ];
            const float& b3 = _e[ 2 ][ 1 ];
            const float& c3 = _e[ 2 ][ 2 ];
            const float& d3 = _e[ 2 ][ 3 ];

            const float& a4 = _e[ 3 ][ 0 ];
            const float& b4 = _e[ 3 ][ 1 ];
            const float& c4 = _e[ 3 ][ 2 ];
            const float& d4 = _e[ 3 ][ 3 ];

            result._e[ 0 ][ 0 ] =  determinant_3x3( b2, b3, b4, c2, c3, c4, d2, d3, d4 );
            result._e[ 1 ][ 0 ] = -determinant_3x3( a2, a3, a4, c2, c3, c4, d2, d3, d4 );
            result._e[ 2 ][ 0 ] =  determinant_3x3( a2, a3, a4, b2, b3, b4, d2, d3, d4 );
            result._e[ 3 ][ 0 ] = -determinant_3x3( a2, a3, a4, b2, b3, b4, c2, c3, c4 );

            result._e[ 0 ][ 1 ] = -determinant_3x3( b1, b3, b4, c1, c3, c4, d1, d3, d4 );
            result._e[ 1 ][ 1 ] =  determinant_3x3( a1, a3, a4, c1, c3, c4, d1, d3, d4 );
            result._e[ 2 ][ 1 ] = -determinant_3x3( a1, a3, a4, b1, b3, b4, d1, d3, d4 );
            result._e[ 3 ][ 1 ] =  determinant_3x3( a1, a3, a4, b1, b3, b4, c1, c3, c4 ); 

            result._e[ 0 ][ 2 ] =  determinant_3x3( b1, b2, b4, c1, c2, c4, d1, d2, d4 );
            result._e[ 1 ][ 2 ] = -determinant_3x3( a1, a2, a4, c1, c2, c4, d1, d2, d4 );
            result._e[ 2 ][ 2 ] =  determinant_3x3( a1, a2, a4, b1, b2, b4, d1, d2, d4 );
            result._e[ 3 ][ 2 ] = -determinant_3x3( a1, a2, a4, b1, b2, b4, c1, c2, c4 );

            result._e[ 0 ][ 3 ] = -determinant_3x3( b1, b2, b3, c1, c2, c3, d1, d2, d3 );
            result._e[ 1 ][ 3 ] =  determinant_3x3( a1, a2, a3, c1, c2, c3, d1, d2, d3 );
            result._e[ 2 ][ 3 ] = -determinant_3x3( a1, a2, a3, b1, b2, b3, d1, d2, d3 );
            result._e[ 3 ][ 3 ] =  determinant_3x3( a1, a2, a3, b1, b2, b3, c1, c2, c3 );

            const float one_over_det = 1.0f / det;
            assert( is_finite( one_over_det ) );

            result._e[ 0 ][ 0 ] *= one_over_det;
            result._e[ 0 ][ 1 ] *= one_over_det;
            result._e[ 0 ][ 2 ] *= one_over_det;
            result._e[ 0 ][ 3 ] *= one_over_det;

            result._e[ 1 ][ 0 ] *= one_over_det;
            result._e[ 1 ][ 1 ] *= one_over_det;
            result._e[ 1 ][ 2 ] *= one_over_det;
            result._e[ 1 ][ 3 ] *= one_over_det;

            result._e[ 2 ][ 0 ] *= one_over_det;
            result._e[ 2 ][ 1 ] *= one_over_det;
            result._e[ 2 ][ 2 ] *= one_over_det;
            result._e[ 2 ][ 3 ] *= one_over_det;

            result._e[ 3 ][ 0 ] *= one_over_det;
            result._e[ 3 ][ 1 ] *= one_over_det;
            result._e[ 3 ][ 2 ] *= one_over_det;
            result._e[ 3 ][ 3 ] *= one_over_det;

            assert( result.is_valid() );

            return result;
        }
      
        //
        // determinant()
        //
        float determinant() const
        {
            const float& a1 = _e[ 0 ][ 0 ];
            const float& b1 = _e[ 0 ][ 1 ];
            const float& c1 = _e[ 0 ][ 2 ];
            const float& d1 = _e[ 0 ][ 3 ];

            const float& a2 = _e[ 1 ][ 0 ];
            const float& b2 = _e[ 1 ][ 1 ];
            const float& c2 = _e[ 1 ][ 2 ];
            const float& d2 = _e[ 1 ][ 3 ];

            const float& a3 = _e[ 2 ][ 0 ];
            const float& b3 = _e[ 2 ][ 1 ];
            const float& c3 = _e[ 2 ][ 2 ];
            const float& d3 = _e[ 2 ][ 3 ];

            const float& a4 = _e[ 3 ][ 0 ];
            const float& b4 = _e[ 3 ][ 1 ];
            const float& c4 = _e[ 3 ][ 2 ];
            const float& d4 = _e[ 3 ][ 3 ];

            return a1 * determinant_3x3( b2, b3, b4, c2, c3, c4, d2, d3, d4 ) -
                   b1 * determinant_3x3( a2, a3, a4, c2, c3, c4, d2, d3, d4 ) +
                   c1 * determinant_3x3( a2, a3, a4, b2, b3, b4, d2, d3, d4 ) -
                   d1 * determinant_3x3( a2, a3, a4, b2, b3, b4, c2, c3, c4 );
        }
        
        //
        // look_at()
        //
        void look_at( const point3& target )
        {
        	vector3 vn( 
        		target._x - _e[ 3 ][ 0 ],
        		target._y - _e[ 3 ][ 1 ],
        		target._z - _e[ 3 ][ 2 ] 
        		);
        		
        	vn.set_length( 1.0 );
        	
        	vector3 vu( 0, 1, 0 );
        	
        	vector3 vr = cross_product( vu, vn );
        	vr.set_length( 1.0 );
        	
        	vu = cross_product( vn, vr );
        	vu.set_length( 1.0 );
        	
        	_e[ 0 ][ 0 ] = vr._dx;
        	_e[ 0 ][ 1 ] = vr._dy;
        	_e[ 0 ][ 2 ] = vr._dz;

        	_e[ 1 ][ 0 ] = vu._dx;
        	_e[ 1 ][ 1 ] = vu._dy;
        	_e[ 1 ][ 2 ] = vu._dz;

        	_e[ 2 ][ 0 ] = vn._dx;
        	_e[ 2 ][ 1 ] = vn._dy;
        	_e[ 2 ][ 2 ] = vn._dz;

            assert( is_valid() );
        }        
    };

    //
    // operator *
    //
    //  x y z 1    	00  01  02  03
    //				10	11	12	13
    //				20	21	22	23
    //				30	31	32	33
    //
    inline point3 operator * ( const point3& a, const matrix44& b )
    {
        assert( a.is_valid() );
        assert( b.is_valid() );

        point3 result;

		result._x = ( a._x * b._e[ 0 ][ 0 ] ) + ( a._y * b._e[ 1 ][ 0 ] ) + ( a._z * b._e[ 2 ][ 0 ] ) + b._e[ 3 ][ 0 ];
		result._y = ( a._x * b._e[ 0 ][ 1 ] ) + ( a._y * b._e[ 1 ][ 1 ] ) + ( a._z * b._e[ 2 ][ 1 ] ) + b._e[ 3 ][ 1 ];
		result._z = ( a._x * b._e[ 0 ][ 2 ] ) + ( a._y * b._e[ 1 ][ 2 ] ) + ( a._z * b._e[ 2 ][ 2 ] ) + b._e[ 3 ][ 2 ];

		const float w = ( a._x * b._e[ 0 ][ 3 ] ) + ( a._y * b._e[ 1 ][ 3 ] ) + ( a._z * b._e[ 2 ][ 3 ] ) + b._e[ 3 ][ 3 ];
		if( w != 0 && w != 1 )
		{
			result._x /= w;
			result._y /= w;
			result._z /= w;
		}

        assert( result.is_valid() );
        return result;
    }

    //
    // operator *
    //
    inline matrix44 operator * ( const matrix44& a, const matrix44& b )
    {
        assert( a.is_valid() );
        assert( b.is_valid() );

        matrix44 result;

        result._e[ 0 ][ 0 ] = ( a( 0, 0 ) * b( 0, 0 ) ) + ( a( 0, 1 ) * b( 1, 0 ) ) + ( a( 0, 2 ) * b( 2, 0 ) ) + ( a( 0, 3 ) * b( 3, 0 ) );
        result._e[ 0 ][ 1 ] = ( a( 0, 0 ) * b( 0, 1 ) ) + ( a( 0, 1 ) * b( 1, 1 ) ) + ( a( 0, 2 ) * b( 2, 1 ) ) + ( a( 0, 3 ) * b( 3, 1 ) );
        result._e[ 0 ][ 2 ] = ( a( 0, 0 ) * b( 0, 2 ) ) + ( a( 0, 1 ) * b( 1, 2 ) ) + ( a( 0, 2 ) * b( 2, 2 ) ) + ( a( 0, 3 ) * b( 3, 2 ) );
        result._e[ 0 ][ 3 ] = ( a( 0, 0 ) * b( 0, 3 ) ) + ( a( 0, 1 ) * b( 1, 3 ) ) + ( a( 0, 2 ) * b( 2, 3 ) ) + ( a( 0, 3 ) * b( 3, 3 ) );
        result._e[ 1 ][ 0 ] = ( a( 1, 0 ) * b( 0, 0 ) ) + ( a( 1, 1 ) * b( 1, 0 ) ) + ( a( 1, 2 ) * b( 2, 0 ) ) + ( a( 1, 3 ) * b( 3, 0 ) );
        result._e[ 1 ][ 1 ] = ( a( 1, 0 ) * b( 0, 1 ) ) + ( a( 1, 1 ) * b( 1, 1 ) ) + ( a( 1, 2 ) * b( 2, 1 ) ) + ( a( 1, 3 ) * b( 3, 1 ) );
        result._e[ 1 ][ 2 ] = ( a( 1, 0 ) * b( 0, 2 ) ) + ( a( 1, 1 ) * b( 1, 2 ) ) + ( a( 1, 2 ) * b( 2, 2 ) ) + ( a( 1, 3 ) * b( 3, 2 ) );
        result._e[ 1 ][ 3 ] = ( a( 1, 0 ) * b( 0, 3 ) ) + ( a( 1, 1 ) * b( 1, 3 ) ) + ( a( 1, 2 ) * b( 2, 3 ) ) + ( a( 1, 3 ) * b( 3, 3 ) );
        result._e[ 2 ][ 0 ] = ( a( 2, 0 ) * b( 0, 0 ) ) + ( a( 2, 1 ) * b( 1, 0 ) ) + ( a( 2, 2 ) * b( 2, 0 ) ) + ( a( 2, 3 ) * b( 3, 0 ) );
        result._e[ 2 ][ 1 ] = ( a( 2, 0 ) * b( 0, 1 ) ) + ( a( 2, 1 ) * b( 1, 1 ) ) + ( a( 2, 2 ) * b( 2, 1 ) ) + ( a( 2, 3 ) * b( 3, 1 ) );
        result._e[ 2 ][ 2 ] = ( a( 2, 0 ) * b( 0, 2 ) ) + ( a( 2, 1 ) * b( 1, 2 ) ) + ( a( 2, 2 ) * b( 2, 2 ) ) + ( a( 2, 3 ) * b( 3, 2 ) );
        result._e[ 2 ][ 3 ] = ( a( 2, 0 ) * b( 0, 3 ) ) + ( a( 2, 1 ) * b( 1, 3 ) ) + ( a( 2, 2 ) * b( 2, 3 ) ) + ( a( 2, 3 ) * b( 3, 3 ) );
        result._e[ 3 ][ 0 ] = ( a( 3, 0 ) * b( 0, 0 ) ) + ( a( 3, 1 ) * b( 1, 0 ) ) + ( a( 3, 2 ) * b( 2, 0 ) ) + ( a( 3, 3 ) * b( 3, 0 ) );
        result._e[ 3 ][ 1 ] = ( a( 3, 0 ) * b( 0, 1 ) ) + ( a( 3, 1 ) * b( 1, 1 ) ) + ( a( 3, 2 ) * b( 2, 1 ) ) + ( a( 3, 3 ) * b( 3, 1 ) );
        result._e[ 3 ][ 2 ] = ( a( 3, 0 ) * b( 0, 2 ) ) + ( a( 3, 1 ) * b( 1, 2 ) ) + ( a( 3, 2 ) * b( 2, 2 ) ) + ( a( 3, 3 ) * b( 3, 2 ) );
        result._e[ 3 ][ 3 ] = ( a( 3, 0 ) * b( 0, 3 ) ) + ( a( 3, 1 ) * b( 1, 3 ) ) + ( a( 3, 2 ) * b( 2, 3 ) ) + ( a( 3, 3 ) * b( 3, 3 ) );

        assert( result.is_valid() );
        return result;
    }

}

#endif // x3d__matrix44_h

