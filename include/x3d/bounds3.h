#ifndef x3d__bounds3_h
#define x3d__bounds3_h

#ifndef x3d__point3_h
#include <x3d/point3.h>
#endif

namespace x3d
{
    //
    // bounds3
    //
    struct bounds3
    {
        point3  _min, _max;
        
    // construction
        bounds3()
        {
        }
        
        bounds3( const point3& min, const point3& max ) :
            _min( min ),
            _max( max )
        {
        }

    // instance methods
        bool is_valid() const
        {
            return ( _min.is_valid() && _max.is_valid() );
        }

        void enclose( point3 a )
        {
            if( is_valid() )
            {
                
            }
            else
            {
                _min = _max = a;
            }
        }
    };

}

#endif // x3d__bounds3_h
