#ifndef x3d__spring_damper_h
#define x3d__spring_damper_h

#ifndef x3d__particle_h
#include <x3d/particle.h>
#endif

namespace x3d
{
    struct spring_damper
    {
        float   _k;     // tensile spring constant
        float   _d;     // damping factor
        float   _L;     // resting length of spring

        spring_damper() :
            _k( nan() ),
            _d( nan() ),
            _L( nan() )
        {
        }

        void calculate_forces( particle& a, particle& b ) const
        {
            const vector3 d = a - b;
            const vector3 v = a._velocity - b._velocity;

            const float   length_of_d = d.length();
            const vector3 f1 = ( d / length_of_d ) * 
                              -( _k * ( length_of_d - _L ) + _d * ( dot_product( v, d ) / length_of_d ) );
            const vector3 f2 = -f1;

            if( a._locked == false )
                a._forces += f1;

            if( b._locked == false )
                b._forces += f2;
        }
    };
}

#endif // x3d__spring_damper_h

