#ifndef x3d__collection_h
#define x3d__collection_h

#ifndef x3d__iteratable_h
#include <x3d/iteratable.h>
#endif

#ifndef x3d__graph_node_h
#include <x3d/graph_node.h>
#endif

namespace x3d
{
    class collection : 
        public graph_node, 
        public graph_node_change_observer, 
        public graph_node_destruction_observer, 
        public iteratable
    {
    public:
        struct collection_data *data;

        collection();
        virtual ~collection();

        // iteratable
        virtual void get_iterator( iterator **result );

        // graph_node_change_observer
        virtual void graph_node_change_triggered( graph_node *subject );
        
        // graph_node_destruction_observer
        virtual void graph_node_destruction_triggered( graph_node *subject );

        // collection
        virtual size_t size() const;
        virtual bool empty() const;
        virtual void append( retainable *item );
        virtual void remove_all();
        
    private:
        collection( collection & );
        collection & operator = ( collection & );        
    };
}

#endif // x3d__collection_h
