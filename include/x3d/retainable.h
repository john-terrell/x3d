#ifndef x3d__retainable_h
#define x3d__retainable_h

namespace x3d
{
    class retainable
    {
    public:
		virtual ~retainable()
		{
		}
		
        virtual int retain() const = 0;
        virtual int release() const = 0;
        virtual void auto_release() const = 0;
    };
}

#endif // x3d__retainable_h
