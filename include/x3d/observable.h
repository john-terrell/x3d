#ifndef x3d__observable_h
#define x3d__observable_h

#ifndef x3d__observer_h
#include <x3d/observer.h>
#endif

#include <set>

namespace x3d
{
    class observer;
    
    class observable
    {
        std::set< observer * > observers;
        
    public:
        void add_observer( observer &o )
        {
            observers.insert( &o );
        }
        
        void remove_observer( observer &o )
        {
            observers.erase( observers.find( &o ) );
        }
        
        void trigger()
        {
            for( std::set< observer * >::iterator i = observers.begin(); i != observers.end(); ++i )
            {
                (*i)->triggered();
            }
        }
    };

    template< class ARG1 >
    class observable1
    {
        typedef observer1< ARG1 > obs1;
        std::set< obs1 * > observers;
        
    public:
        void add_observer( observer1< ARG1 > &o )
        {
            observers.insert( &o );
        }
        
        void remove_observer( observer1< ARG1 > &o )
        {
            observers.erase( observers.find( &o ) );
        }
        
        void trigger( ARG1 arg1 )
        {
			typename std::set< obs1 * >::iterator i;
            for(  i = observers.begin(); i != observers.end(); ++i )
            {
                (*i)->triggered( arg1 );
            }
        }
    };

}

#endif // x3d__observable_h
