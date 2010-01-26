#ifndef x3d__particle_h
#define x3d__particle_h

#ifndef x3d__vector3_h
#include <x3d/vector3.h>
#endif

#ifndef x3d__point3_h
#include <x3d/point3.h>
#endif

namespace x3d
{
    struct particle : point3
    {
        vector3 _velocity;
        vector3 _acceleration;
        vector3 _forces;        // current forces that haven't been applied

        bool    _locked;        // if true, position can't change

        particle() : _locked( false )
        {
        }
    };
}

#endif // x3d__particle_h
