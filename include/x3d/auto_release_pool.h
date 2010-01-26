#ifndef x3d__auto_release_pool_h
#define x3d__auto_release_pool_h

namespace x3d
{
    class retainable;
    
    class auto_release_pool
    {
        struct auto_release_pool_data *data;
        
    public:
        auto_release_pool();
        virtual ~auto_release_pool();
        
        void add_item( const retainable *item );
        void release_all_items();
        
    private:
        auto_release_pool( const auto_release_pool & );
        auto_release_pool & operator = ( const auto_release_pool & );
        
    public:
        static auto_release_pool *default_auto_release_pool;        
    };
}

#endif // x3d__auto_release_pool_h