#ifndef x3d__transformation_h
#define x3d__transformation_h

#ifndef x3d__graph_node_h
#include <x3d/graph_node.h>
#endif

#ifndef x3d__renderable_h
#include <x3d/renderable.h>
#endif

#ifndef x3d__matrix44_h
#include <x3d/matrix44.h>
#endif

namespace x3d
{
    class transformation : public graph_node, public renderable
    {
    protected:
        matrix44 _matrix;
        
    public:
        transformation();
        
        // renderable
        virtual void render();

        // transformation
        virtual const matrix44 &matrix()
        {
            return _matrix;
        }
                
    private:
        transformation( const transformation & );
        transformation & operator = ( const transformation & );        
    };
}

#endif // x3d__transformation_h
