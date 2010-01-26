#ifndef test1__window_h
#define test1__window_h

#ifndef x3d__field_h
#include <x3d/field.h>
#endif

#ifndef x3d__graph_node_h
#include <x3d/graph_node.h>
#endif

class window : public x3d::graph_node_change_observer
{
    int glut_id;
    struct window_data *data;
    
public:
    x3d::field< float > _rotation_angle;
    
public:
    window( int x, int y, unsigned width, unsigned height );
    virtual ~window();
    
    virtual void graph_node_change_triggered( x3d::graph_node *subject );
    
    virtual void refresh();
};

#endif // test1__window_h
