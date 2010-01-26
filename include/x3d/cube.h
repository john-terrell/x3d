#ifndef x3d__cube_h
#define x3d__cube_h

#ifndef x3d__graph_node_h
#include <x3d/graph_node.h>
#endif

#ifndef x3d__renderable_h
#include <x3d/renderable.h>
#endif

namespace x3d
{
    class cube : public graph_node, public renderable
    {
    public:
        cube();

        // renderable
        virtual void render();

    private:
        cube( const cube & );
        cube & operator = ( const cube & );        
    };
}

#endif // x3d__cube_h
