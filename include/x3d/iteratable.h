#ifndef x3d__iteratable_h
#define x3d__iteratable_h

namespace x3d
{
    class iterator;

    class iteratable
    {
    public:
		virtual ~iteratable()
		{
		}
		
        virtual void get_iterator( iterator **result ) =  0;
    };
}

#endif // x3d__iteratable_h
