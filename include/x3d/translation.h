#ifndef x3d__translation_h
#define x3d__translation_h

#ifndef x3d__transformation_h
#include <x3d/transformation.h>
#endif

#ifndef x3d__vector3_h
#include <x3d/vector3.h>
#endif

namespace x3d
{
    class translation : public transformation
    {
    public:
        translation( const vector3 &initial_offset );

        // translation
        const matrix44 &matrix();
        void translate( const vector3 &offset );

    public:
        field< vector3 > _translation;
        
    private:
        translation( const translation & );
        translation& operator = ( const translation & );        
    };
}

#endif // x3d__translation_h
