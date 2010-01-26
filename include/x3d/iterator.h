#ifndef x3d__iterator_h
#define x3d__iterator_h

namespace x3d
{
    class iterator : public retainable
    {
    public:
        virtual bool move_forward() = 0;
        virtual void current( retainable **current_item ) const = 0;
    };
}

#endif // x3d__iterator_h
