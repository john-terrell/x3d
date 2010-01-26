#ifndef x3d__observer_h
#define x3d__observer_h

namespace x3d
{
    class observer
    {
    public:
        virtual void triggered() = 0;
    };

    template< class ARG1 >
    class observer1
    {
    public:
		virtual ~observer1()
		{
		}
		
        virtual void triggered( ARG1 arg1 ) = 0;
    };
}

#endif // x3d__observer_h
