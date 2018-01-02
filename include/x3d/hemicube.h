#ifndef x3d__radiosity__hemicube_h
#define x3d__radiosity__hemicube_h

#include <vector>

namespace x3d
{
    class hemicube
    {
        std::vector< float > top_form_factors;
        std::vector< float > side_form_factors;
        unsigned _resolution;
        
    public:
        hemicube( unsigned resolution );
        
        float delta_form_factor_top( unsigned x, unsigned y );
        float delta_form_factor_side( unsigned x, unsigned y );
    };
}

#endif // x3d__radiosity__hemicube_h

