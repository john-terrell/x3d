#include <x3d/x3d.h>
#include <x3d/collection.h>
#include <x3d/iterator.h>
#include <x3d/auto_release_pool.h>
#include <x3d/reference.h>

#include <list>

using std::list;
using namespace x3d;

struct x3d::collection_data
{
    list< retainable * > items;
};

class collection_iterator : public iterator
{
    mutable int refs;
    bool initialized;
    list< retainable * >::iterator i;
    list< retainable * >::iterator begin;
    list< retainable * >::iterator end;
    
public:
    collection_iterator( collection &a ) : 
        refs( 1 ),
        initialized( false ),
        begin( a.data->items.begin() ),
        end( a.data->items.end() )
    {
    }

    int retain() const
    {
        return ++refs;
    }
    
    int release() const
    {
        const int r = --refs;
        if( r == 0 )
            delete this;
            
        return r;
    }

    void auto_release() const
    {
        auto_release_pool::default_auto_release_pool->add_item( this );
    }

    // iterator
    bool move_forward()
    {
        bool is_valid = true;
        if( initialized )
            ++i;
        else
        {
            i = begin;
            initialized = true;
        }
        
        if( i == end )
            is_valid = false;
        
        return is_valid;
    }
    
    void current( retainable **current_item ) const;
};

void collection_iterator :: current( retainable **current_item ) const
{
    assert( current_item );
    assert( i != end );
    *current_item = *i;
    (*current_item)->retain();
}

collection :: collection() : data( 0 )
{
    data = new collection_data;
}

collection :: ~collection()
{
    remove_all();

    delete data;
    data = 0;
}

void collection :: graph_node_change_triggered( graph_node * )
{
    _changed.trigger( this );
}

void collection :: graph_node_destruction_triggered( graph_node *subject )
{
    subject->remove_change_observer( *this );
}

void collection :: get_iterator( iterator **result )
{
    assert( result );
    *result = new collection_iterator( *this );
}

size_t collection :: size() const
{
    return data->items.size();
}

bool collection :: empty() const
{
    return data->items.empty();
}

void collection :: append( retainable *item )
{
    assert( item );
    
    item->retain();
    data->items.push_back( item );
    
    graph_node *node = dynamic_cast< graph_node * >( item );
    if( node )
    {
        node->add_change_observer( *this );
        node->add_destruction_observer( *this );
    }
    
    _changed.trigger( this );
}

void collection :: remove_all()
{
    while( !data->items.empty() )
    {
        retainable *item = *(data->items.begin());

        graph_node *node = dynamic_cast< graph_node * >( item );
        if( node )
        {
            node->remove_destruction_observer( *this );
            node->remove_change_observer( *this );
        }

        assert( item );
        item->release();
        data->items.erase( data->items.begin() );        
    }
    
    _changed.trigger( this );
}
