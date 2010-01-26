#include <x3d/x3d.h>
#include <x3d/auto_release_pool.h>
#include <x3d/retainable.h>
#include <vector>

using namespace std;
using namespace x3d;

//
// the default auto release pool
//
static auto_release_pool default_pool;
auto_release_pool *auto_release_pool::default_auto_release_pool = &default_pool;

struct x3d::auto_release_pool_data
{
    vector< const retainable * > items;
};

auto_release_pool :: auto_release_pool() : data( 0 )
{
    data = new auto_release_pool_data;
}

auto_release_pool :: ~auto_release_pool()
{
    release_all_items();

    delete data;
    data = 0;
}

void auto_release_pool :: add_item( const retainable *item )
{
    data->items.push_back( item );
}

void auto_release_pool :: release_all_items()
{
    while( data->items.size() > 0 )
    {
        data->items.back()->release();
        data->items.back() = 0;
        
        data->items.pop_back();
    }
}
