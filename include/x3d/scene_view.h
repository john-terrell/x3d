#ifndef x3d__scene_view_h
#define x3d__scene_view_h

#ifndef x3d__collection_h
#include <x3d/collection.h>
#endif

namespace x3d
{
    class camera;
    
    class scene_view : public collection
    {
        struct scene_view_data *data;
        
    public:
        scene_view();
        virtual ~scene_view();
        
        // scene_view
        virtual void render( int width, int height );

    private:
        scene_view( const scene_view & );
        scene_view & operator = ( const scene_view & );        
    };
}

#endif // x3d__scene_view_h
