#ifndef x3d__sphere_h
#define x3d__sphere_h

#ifndef x3d__graph_node_h
#include <x3d/graph_node.h>
#endif

#ifndef x3d__renderable_h
#include <x3d/renderable.h>
#endif

namespace x3d
{
    class sphere : public graph_node, public renderable
    {
    public:
        sphere();
        
        // renderable
        virtual void render();
        
    private:
        sphere( const sphere & );
        sphere & operator = ( const sphere & );        
    };
}

#endif // x3d__sphere_h
