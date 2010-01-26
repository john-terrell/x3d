#ifndef x3d__scene_h
#define x3d__scene_h

#ifndef x3d__collection_h
#include <x3d/collection.h>
#endif

#ifndef x3d__field_h
#include <x3d/field.h>
#endif

namespace x3d
{
    class scene_view;

    class scene : public collection
    {
        struct scene_data *_data;
        
    public:
        scene();
        virtual ~scene();

        // scene
        void create_view( scene_view **new_view );
        void root_collection( collection &root_collection );
        
        void timer_callback();
        
    public: // fields
        field< bool > _modified;
        
        field< unsigned long > _current_time;
        field< unsigned long > _time_loop_minimum;
        field< unsigned long > _time_loop_maximum;
    };
}

#endif // x3d__scene_h
