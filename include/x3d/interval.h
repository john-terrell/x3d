#ifndef x3d__interval_h
#define x3d__interval_h

namespace x3d
{
    //
    // interval
    //
    struct interval
    {
        float _min, _max;

    // construction
        interval() :
            _min( nan() ),
            _max( nan() )
        {
        }

        interval( float minimum, float maximum ) :
            _min( minimum ),
            _max( maximum )
        {
            assert( is_finite( minimum ) );
            assert( is_finite( maximum ) );
        }
        
    // instance methods
        bool is_valid() const
        {
            return ( is_finite( _min ) && is_finite( _max ) && _min <= _max ? true : false );
        }
        
        //
        // contains()
        //
        bool contains( float a ) const
        {
            assert( is_valid() );
            assert( is_finite( a ) );
            
            return ( a >= _min && a <= _max ? true : false );
        }
    };

}

#endif // x3d__interval_h
