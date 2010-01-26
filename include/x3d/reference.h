#ifndef x3d__reference_h
#define x3d__reference_h

namespace x3d
{
    template < class T >
    class reference
    {
    public:
        T*  ref;

        reference( T* a = 0 ) : ref( 0 )
        {
            attach( a );
        }
        
        reference( const reference< T >& a ) :
            ref( 0 )
        {
            attach( a.ref );
        }

        virtual ~reference() throw()
        {
            detach();
        }

        reference< T >& operator = ( const reference< T >& a ) 
        {
            if( a.ref != ref )
            {
                detach();

                attach( a.ref );
            }

            return *this;
        }

        T** operator & () 
        {
            return &ref;
        }

        const T** operator & () const 
        {
            return &ref;
        }

        const T* operator -> () const
        {
            return ref;
        }

        T* operator -> ()
        {
            return ref;
        }

        const T& operator * () const
        {
            assert( ref );
            return *ref;
        }

        T& operator * ()
        {
            assert( ref );
            return *ref;
        }

        operator T* () const
        {
            return ref;
        }

        void detach()
        {
            if( ref )
            {
                ref->release();
                ref = 0;
            }
        }

        void attach( T* ref )
        {
            detach();

            if( ref )
            {
                this->ref = ref;
                this->ref->retain();
            }
        }
    };
}

#endif // x3d__reference_h
