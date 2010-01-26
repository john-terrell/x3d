#include <x3d/x3d.h>
#include <x3d/graph_node.h>
#include <x3d/auto_release_pool.h>
#include <stdio.h>

using namespace x3d;

class graph_node_count_validator
{
public:
    ~graph_node_count_validator()
    {
        int instance_count = graph_node::instance_count;
        if( instance_count > 0 )
        {
            fprintf( stderr,"ERROR: Leaking graph node objects: %d\n", instance_count );
            assert( instance_count == 0 );
        }
    }
};

static graph_node_count_validator ccv;  // this object will validate that all of our
                                        // graph_nodes are destroyed (not being leaked)
unsigned long graph_node::instance_count = 0;

graph_node :: graph_node() : _refs( 0 )
{
    retain();
    ++instance_count;
}

graph_node :: ~graph_node()
{
    assert( _refs == 0 );
    --instance_count;
    _destroyed.trigger( this );
}

int graph_node :: retain() const
{
    return ++_refs;
}

int graph_node :: release() const
{
    const int r = --_refs;
    if( r == 0 )
        delete this;
        
    return r;
}

void graph_node :: auto_release() const
{
    auto_release_pool::default_auto_release_pool->add_item( this );
}

void graph_node :: field_change_triggered()
{
    _changed.trigger( this );
}

void graph_node :: watch_field_for_changes( abstract_field *field )
{
    field->add_field_change_observer( *this );
}

void graph_node :: add_change_observer( graph_node_change_observer &observer )
{
    _changed.add_observer( observer );
}

void graph_node :: remove_change_observer( graph_node_change_observer &observer )
{
    _changed.remove_observer( observer );
}

void graph_node :: add_destruction_observer( graph_node_destruction_observer &observer )
{
    _destroyed.add_observer( observer );
}

void graph_node :: remove_destruction_observer( graph_node_destruction_observer &observer )
{
    _destroyed.remove_observer( observer );
}
