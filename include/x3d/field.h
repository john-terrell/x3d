#ifndef x3d__field_h
#define x3d__field_h

#ifndef x3d__retainable_h
#include <x3d/retainable.h>
#endif

#ifndef x3d__reference_h
#include <x3d/reference.h>
#endif

#ifndef x3d__observable_h
#include <x3d/observable.h>
#endif

#ifndef x3d__observer_h
#include <x3d/observer.h>
#endif

namespace x3d
{
    class abstract_field;
    
    class field_change_observer : public observer
    {
    public:
		virtual ~field_change_observer()
		{
		}
		
        virtual void field_change_triggered() = 0;
        
        virtual void triggered()
        {
            field_change_triggered();
        }
    };
    
    class field_refresh_observer : public observer
    {
    public:
		virtual ~field_refresh_observer()
		{
		}
		
        virtual void field_refresh_triggered() = 0;
        
        virtual void triggered()
        {
            field_refresh_triggered();
        }
    };
    
    class field_destruction_observer : public observer1< abstract_field * >
    {
    public:
        virtual void field_destruction_triggered( abstract_field *subject ) = 0;
        
        virtual void triggered( abstract_field *subject )
        {
            field_destruction_triggered( subject );
        }
    };

    class abstract_field
    {
    protected:
        mutable bool _stale;
        mutable unsigned long _most_recent_change_serial_number;
        
        mutable observable _refresh_from_upstream;
        observable _field_changed;
        observable1< abstract_field * > _destroyed;
                
    public:
        abstract_field() : _stale( false ), _most_recent_change_serial_number( 0 )
        {
        }
        
        virtual ~abstract_field()
        {
            _destroyed.trigger( this );
        }
        
        bool is_stale() const
        {
            return _stale;
        }
        
        void set_stale( bool stale )
        {
            if( _stale != stale )
            {
                _stale = stale;
                if( _stale )
                    _field_changed.trigger();
            }
        }
        
        void refresh() const
        {
            _refresh_from_upstream.trigger();
            _stale = false;
        }
        
        void changed()
        {
            _field_changed.trigger();
        }
        
        void add_field_change_observer( field_change_observer &o )
        {
            _field_changed.add_observer( o );
        }
        
        void remove_field_change_observer( field_change_observer &o )
        {
            _field_changed.remove_observer( o );
        }
        
        void add_refresh_observer( field_refresh_observer &o )
        {
            _refresh_from_upstream.add_observer( o );
        }
        
        void remove_refresh_observer( field_refresh_observer &o )
        {
            _refresh_from_upstream.remove_observer( o );
        }
        
        void add_field_destruction_observer( field_destruction_observer &o )
        {
            _destroyed.add_observer( o );
        }

        void remove_field_destruction_observer( field_destruction_observer &o )
        {
            _destroyed.remove_observer( o );
        }
        
    public:
        static unsigned long current_change_serial_number;
        static unsigned long connector_count;
    };

    template < class T >
    class field : public abstract_field
    {
        T _value;
        
    public:
        field( T value ) :abstract_field()
        {
            _value = value;
        }
        
        virtual ~field()
        {
        }
        
        T get_value() const
        {
            if( _stale )
                refresh();
                
            return _value;
        }
        
        void set_value( T value )
        {
            if( _value != value )
            {
                _value = value;
                changed();
            }
        }        
    };
    
    template < class FIELD1_TYPE, class FIELD2_TYPE >
    class field_connector : public field_change_observer, public field_refresh_observer, public field_destruction_observer
    {
        field< FIELD1_TYPE > *_upstream_field;
        field< FIELD2_TYPE > *_downstream_field;
        
    public:
        field_connector( field< FIELD1_TYPE > &upstream_field, field< FIELD2_TYPE > &downstream_field ) :
            _upstream_field( &upstream_field ),
            _downstream_field( &downstream_field )
        {
            ++abstract_field::connector_count;
            
            upstream_field.add_field_change_observer( *this );
            downstream_field.add_refresh_observer( *this );
            
            upstream_field.add_field_destruction_observer( *this );
            downstream_field.add_field_destruction_observer( *this );
        }
        
        virtual ~field_connector()
        {
            --abstract_field::connector_count;
        }
        
        virtual void field_refresh_triggered()
        {
            _downstream_field->set_value( _upstream_field->get_value() );
        }
        
        virtual void field_change_triggered()
        {
            _downstream_field->set_stale( true );
        }
        
        virtual void field_destruction_triggered( abstract_field *subject )
        {
            //
            // one of the connected fields is being destroyed,
            // remove the connection and deletion the connector object
            //
            _upstream_field->remove_field_change_observer( *this );
            _downstream_field->remove_refresh_observer( *this );

            //
            // only remove this object from the field's destruction observable
            // that did NOT trigger us (since that
            // field is going away, we don't want to erase ourselves from its
            // observer table because it's in the middle of traversing it calling
            // observers)
            //
            if( subject != _upstream_field )
                _upstream_field->remove_field_destruction_observer( *this );
            else
                _downstream_field->remove_field_destruction_observer( *this );

            _upstream_field = 0;
            _downstream_field = 0;
            
            delete this;
        }
    };

    template < class FIELD1_TYPE, class FIELD2_TYPE >
    void field_connect( field< FIELD1_TYPE > &a, field< FIELD2_TYPE > &b )
    {
        new field_connector< FIELD1_TYPE, FIELD2_TYPE >( a, b );
        
        b.set_stale( true );
    }
};

#endif // x3d__field_h
