#ifndef x3d__quaternion_h
#define x3d__quaternion_h

#ifndef x3d__vector3_h
#include <x3d/vector3.h>
#endif

#ifndef x3d__matrix44_h
#include <x3d/matrix44.h>
#endif

namespace x3d
{
	//
	// quaternion
	//
	struct quaternion
	{
		vector3 _v;	// x, y, z
		float _s;	// w
	
	// construction
        quaternion() :
            _s( nan() )
		{
		}
	
		quaternion( const vector3& axis, float angle )
		{
            assert( axis.is_valid() );
            assert( is_finite( angle ) );

			set( axis, angle );

            assert( is_valid() );
		}
	
	// instance methods
        bool is_valid() const
        {
            return ( _v.is_valid() && is_finite( _s ) ? true : false );
        }

		void set( const vector3& axis, float angle )
		{
            assert( axis.is_valid() );
            assert( is_finite( angle ) );

			_s = cos( angle * 0.5f );
		
			const float sn = sin( angle * 0.5f );
            assert( is_finite( sn ) );

			_v._dx = axis._dx * sn;
			_v._dy = axis._dy * sn;
			_v._dz = axis._dz * sn;
		}
		
		void invert()
		{
            assert( is_valid() );

			_v._dx = -_v._dx;
			_v._dy = -_v._dy;
			_v._dz = -_v._dz;
		}
	
		void create_matrix44( matrix44& m ) const
		{
            assert( is_valid() );

			const float& w = _s;
			const float& x = _v._dx;
			const float& y = _v._dy;
			const float& z = _v._dz;
		
			const float w2 = w * w;
			const float x2 = _v._dx * _v._dx;
			const float y2 = _v._dy * _v._dy;
			const float z2 = _v._dz * _v._dz;
		
			m._e[ 0 ][ 0 ] = w2 + x2 - y2 - z2;
			m._e[ 0 ][ 1 ] = ( 2.0f * x * y ) + ( 2.0 * w * z );
			m._e[ 0 ][ 2 ] = ( 2.0f * x * z ) - ( 2.0 * w * y );
			m._e[ 0 ][ 3 ] = 0.0f;
		
			m._e[ 1 ][ 0 ] = ( 2.0f * x * y ) - ( 2.0 * w * z );
			m._e[ 1 ][ 1 ] = w2 - x2 + y2 - z2;
			m._e[ 1 ][ 2 ] = ( 2.0f * y * z ) + ( 2.0 * w * x );
			m._e[ 1 ][ 3 ] = 0.0f;
		
			m._e[ 2 ][ 0 ] = ( 2.0f * x * z ) + ( 2.0 * w * y );
			m._e[ 2 ][ 1 ] = ( 2.0f * y * z ) - ( 2.0 * w * x );
			m._e[ 2 ][ 2 ] = w2 - x2 - y2 + z2;
			m._e[ 2 ][ 3 ] = 0.0f;
	
			m._e[ 3 ][ 0 ] = 0;	
			m._e[ 3 ][ 1 ] = 0;	
			m._e[ 3 ][ 2 ] = 0;	
			m._e[ 3 ][ 3 ] = w2 + x2 + y2 + z2;	

            assert( m.is_valid() );
		}
	
	// operators
		operator matrix44 () const
		{
			matrix44 m;

			create_matrix44( m );
		
			return m;
		}
	};

	inline quaternion operator * ( const quaternion& a, const quaternion& b )
	{
        assert( a.is_valid() );
        assert( b.is_valid() );

        quaternion q;

		q._s = ( a._s * b._s ) - dot_product( a._v, b._v );
	
		const vector3 cross = cross_product( a._v, b._v );
	
		q._v._dx = ( a._s * b._v._dx ) + ( b._s * a._v._dx ) + cross._dx;
		q._v._dy = ( a._s * b._v._dy ) + ( b._s * a._v._dy ) + cross._dy;
		q._v._dz = ( a._s * b._v._dz ) + ( b._s * a._v._dz ) + cross._dz;

        return q;
	}

} // namespace x3d

#endif // x3d__quaternion_h
