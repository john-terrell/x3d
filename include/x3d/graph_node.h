#ifndef x3d__graph_node_h
#define x3d__graph_node_h

#ifndef x3d__retainable_h
#include <x3d/retainable.h>
#endif

#ifndef x3d__field_h
#include <x3d/field.h>
#endif

namespace x3d
{
    class graph_node;
    
    class graph_node_change_observer : public observer1< graph_node * >
    {
    public:
		virtual ~graph_node_change_observer()
		{
		}
		
        virtual void graph_node_change_triggered( graph_node *subject ) = 0;
        
        virtual void triggered( graph_node *subject )
        {
            graph_node_change_triggered( subject );
        }
    };

    class graph_node_destruction_observer : public observer1< graph_node * >
    {
    public:
        virtual void graph_node_destruction_triggered( graph_node *subject ) = 0;
        
        virtual void triggered( graph_node *subject )
        {
            graph_node_destruction_triggered( subject );
        }
    };

    class graph_node : 
        public field_change_observer,
        public retainable
    {
        mutable int _refs;
        
    protected:
        observable1< graph_node * > _destroyed;
        observable1< graph_node * > _changed;
          
    protected:
        graph_node();
        virtual ~graph_node();
        
        void watch_field_for_changes( abstract_field *subject );
        
    public:
        void add_change_observer( graph_node_change_observer &observer );
        void remove_change_observer( graph_node_change_observer &observer );

        void add_destruction_observer( graph_node_destruction_observer &observer );
        void remove_destruction_observer( graph_node_destruction_observer &observer );
    
        // retainable
        virtual int retain() const;
        virtual int release() const;
        virtual void auto_release() const;
        
        // field_change_observer
        virtual void field_change_triggered();
        
    public:
        static unsigned long instance_count;
        
    private:
        graph_node( const graph_node & );
        graph_node & operator = ( const graph_node & );
    };
}

#endif // x3d__graph_node_h
