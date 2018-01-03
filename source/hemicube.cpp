#include "x3d.h"
#include "hemicube.h"

hemicube :: hemicube( unsigned resolution ) :
    _resolution( resolution )
{
    assert( ( resolution & 1 ) == 0 ); // resolution must be an even number
            
    top_form_factors.resize( _resolution * _resolution );
    side_form_factors.resize( ( _resolution * _resolution ) >> 1 );
    
    const float pixel_area = ( 2.0f / _resolution ) * ( 2.0f / _resolution );
    const float half_pixel_width = 1.0f / _resolution;
    
    for( unsigned x = 0; x < _resolution; ++x )
    {
        for( unsigned y = 0; y < _resolution; ++y )
        {
            int ym = y - (_resolution/2);
            int xm = x - (_resolution/2);

            float dx = xm / (_resolution/2.0f);
            float dy = ym / (_resolution/2.0f);

            float xSquare = (dx+half_pixel_width)*(dx+half_pixel_width);
            float ySquare = (dy+half_pixel_width)*(dy+half_pixel_width);

            float delta_form_factor = pixel_area/((float)M_PI*(xSquare + ySquare + 1)*(xSquare + ySquare + 1.0f));

            top_form_factors[ x + ( y * _resolution ) ] = delta_form_factor;

        }
    }

    for( unsigned y = 0; y < _resolution; ++y )
    {
        for( unsigned z = 0; z < _resolution >> 1; ++z )
        {
            int ym = y - (_resolution/2);

            float dy = ym / (_resolution/2.0f);
            float dz = z / (_resolution/2.0f);

            float ySquare = (dy+half_pixel_width)*(dy+half_pixel_width);
            float zSquare = (dz+half_pixel_width)*(dz+half_pixel_width);

            float delta_form_factor = (pixel_area*(dz+half_pixel_width))/((float)M_PI*(ySquare + zSquare + 1)*(ySquare +
zSquare + 1));
            side_form_factors[ y + ( z * _resolution ) ] = delta_form_factor;
        }
    }
    
}

float hemicube :: delta_form_factor_top( unsigned row, unsigned col )
{
    const unsigned index = col + ( row * _resolution );

    return top_form_factors[ index ];
}

float hemicube :: delta_form_factor_side( unsigned row, unsigned col )
{
    const unsigned index = col + ( row * _resolution );

    return side_form_factors[ index ];
}

