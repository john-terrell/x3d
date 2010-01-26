#ifndef x3d__rotation_h
#define x3d__rotation_h

#ifndef x3d__transformation_h
#include <x3d/transformation.h>
#endif

namespace x3d
{
    class rotation : public transformation
    {
    public:
        field< float > _angle;
        field< float > _dx;
        field< float > _dy;
        field< float > _dz;
        
    public:
        rotation( float iangle = 0.0f, float idx = 0.0f, float idy = 1.0f, float idz = 0.0f );

        // rotation
        const matrix44 &matrix();
        void rotate( float angle_delta );

    private:
        rotation( const rotation & );
        rotation & operator = ( const rotation & );        
    };
}

#endif // x3d__rotation_h
