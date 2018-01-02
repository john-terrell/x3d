#include "x3d.h"
#include "opengl.h"
#include "radiosity.h"

#include <stdio.h>
#include <cmath>

using namespace x3d;

const float absorption = 0.3;

static void redisplay()
{
}

radiosity :: radiosity( int parent_window, unsigned resolution ) :
    _total_flux( NAN ),
    _total_unsent_flux( 0 ),
    _resolution( resolution ),
    _hemicube( resolution ),
    _steps( 0 )
{
    _glut_id = glutCreateWindow( "Formfactor Window" );
    glutReshapeWindow( resolution, resolution );
    glutDisplayFunc( redisplay );
    glutShowWindow();
    
    _vertices.reserve( 1000000 );
    _patches.reserve( 100000 );
    
    _elements.reserve( 1000000 );
    _form_factors.reserve( _elements.capacity() );
}

float radiosity :: convergence() const
{
    float convergence = 0.0f;
    if (std::isnan(_total_flux))
    {
        convergence = 1.0f;
    }
    else if( _total_flux > 0.001 )
    {
        convergence = fabs( _total_unsent_flux ) / _total_flux;
    }       
    return convergence;
}

void radiosity :: calculate_frame_from_element( const element& e, vector3 frame[ 3 ] )
{
    frame[ 0 ] = e.normal();
    frame[ 0 ].set_length( 1.0f );
    
    do
    {
        const vector3 r = random_vector();
        frame[ 1 ] = cross_product( frame[ 0 ], r );
    }
    while( frame[ 1 ].length() < 0.01 );
    
    frame[ 1 ].set_length( 1.0f );
    frame[ 2 ] = cross_product( frame[ 0 ], frame[ 1 ] );
	frame[ 0 ].set_length( -1.0f );
}

void radiosity :: calculate_form_factors( unsigned patch_index )
{
    //
    // initialize the form factors of each element
    //
    for( std::vector< float >::iterator i = _form_factors.begin(); i != _form_factors.end(); ++i )
    {
        *i = 0.0f;
    }
    
    const element &e = _patches[ patch_index ];
    
    vector3 frame[ 3 ];
    calculate_frame_from_element( e, frame );
    
    //
    // render the top face of the hemicube
    //
    render( patch_index, frame[ 0 ], frame[ 2 ] );
    
    std::vector< unsigned > pixelBuffer;
    pixelBuffer.resize( _resolution );
    
    glFinish();
    
    glReadBuffer( GL_BACK );

    for( unsigned row = 0; row < _resolution; ++row )
    {    
#ifdef _WIN32
		glReadPixels( 0, row, _resolution, 1, GL_RGBA, GL_BYTE, &pixelBuffer[ 0 ] );
#else
        glReadPixels( 0, row, _resolution, 1, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, &pixelBuffer[ 0 ] );
#endif

        unsigned *pixel = &pixelBuffer[ 0 ];
        for( unsigned col = 0; col < _resolution; ++col )
        {
            unsigned index = *pixel >> 8; // lop off the alpha channel
            assert( index == 0xFFFFFF || index <= _form_factors.size() );
            
            if( index != 0xFFFFFF )
                _form_factors[ index ] += _hemicube.delta_form_factor_top( row, col );
        
            ++pixel;
        }
    }

    glutSwapBuffers();

    for( unsigned side_face = 0; side_face < 4; ++side_face )
    {
        vector3 view;
        
        switch( side_face )
        {
            case 0:
                view = frame[ 1 ];
                break;
                
            case 1:
                view = -frame[ 1 ];
                break;
                
            case 2:
                view = frame[ 2 ];
                break;
                
            case 3:
                view = -frame[ 2 ];
                break;
        }
        
        render( patch_index, view, frame[ 0 ] );
        
        for( unsigned row = 0; row < _resolution >> 1; ++row )
        {    
            unsigned read_row = ( _resolution >> 1 ) + row;
#ifdef _WIN32
			glReadPixels( 0, read_row, _resolution, 1, GL_RGBA, GL_BYTE, &pixelBuffer[ 0 ] );
#else
			glReadPixels( 0, read_row, _resolution, 1, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, &pixelBuffer[ 0 ] );
#endif                        
            unsigned *pixel = &pixelBuffer[ 0 ];
            for( unsigned col = 0; col < _resolution; ++col )
            {
                unsigned index = *pixel >> 8; // lop off the alpha channel
                assert( index == 0xFFFFFF || index <= _form_factors.size() );
                
                unsigned row2 = row; // ( _resolution >> 1 ) - 1 - row;
                
                if( index != 0xFFFFFF )
                    _form_factors[ index ] += _hemicube.delta_form_factor_side( row2, col );
            
                ++pixel;
            }
        }
    
        glutSwapBuffers();

    }
}

void radiosity :: execute_iteration()
{
    ++_steps;

    //
    // set the glut window to our formfactor output window
    //
    int current_glut_id = glutGetWindow();
    glutSetWindow( _glut_id );
    
    _index_of_patch_with_highest_unsent_flux = determine_patch_with_highest_unsent_flux(_total_unsent_flux);
    if (std::isnan(_total_flux))
    {
        _total_flux = _total_unsent_flux;
    }

    calculate_form_factors( _index_of_patch_with_highest_unsent_flux );
    
    //
    // walk the element list and if that element's form factor entry in 
    // the table is greater than zero, set its exitance
    //
    for( unsigned index = 0; index < _elements.size(); ++index )
    {
        if( _form_factors[ index ] == 0.0 )
            continue;
            
        float rff = _form_factors[ index ] * _patches[ _index_of_patch_with_highest_unsent_flux ].area() / _elements[ index ].area();
        rff = ( rff > 1.0 ? 1.0 : rff );
        
        if( rff > 0.0 )
        {
            color shooting_exitance = _patch_exitances[ _index_of_patch_with_highest_unsent_flux ];
            color delta = _elements[ index ].reflectance() * rff * shooting_exitance;
        
            _exitances[ index ] += delta * ( 1.0 - absorption );
            
            delta *= _elements[ index ].area() / _patches[ _element_owners[ index ] ].area();
            
            _patch_exitances[ _element_owners[ index ] ] += delta * ( 1.0 - absorption );
        }
    }
    
    _patch_exitances[ _index_of_patch_with_highest_unsent_flux ] = color( 0, 0, 0 );

//    _index_of_patch_with_highest_unsent_flux = determine_patch_with_highest_unsent_flux(_total_unsent_flux);

    glutSetWindow( current_glut_id );
}

void radiosity :: render( unsigned patch_index, const vector3 &view, const vector3 &up )
{
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable( GL_DEPTH_TEST );
    glShadeModel( GL_FLAT );
    glPolygonMode( GL_FRONT, GL_FILL );
    glPolygonMode( GL_BACK, GL_FILL );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glViewport( 0, 0, _resolution, _resolution );
	
    gluPerspective( 90, 1, 10, 1000 );
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    const element &shooter = _patches[ patch_index ];
    
    gluLookAt( shooter.centroid()._x, 
               shooter.centroid()._y, 
               shooter.centroid()._z,
                shooter.centroid()._x + view._dx,
                shooter.centroid()._y + view._dy,
                shooter.centroid()._z + view._dz,
                up._dx,
                up._dy,
                up._dz 
                );
    
    glBegin( GL_TRIANGLES );
    
    for( unsigned index = 0; index < _elements.size(); ++index )
    {
        // If this is the patch we're rendering from, skip it.
        if( patch_index == _element_owners[ index ] )
            continue;
        
        element &e = _elements[ index ];
        
        const unsigned r = ( index & 0xFF0000 ) >> 16;
        const unsigned g = ( index & 0x00FF00 ) >> 8;
        const unsigned b = ( index & 0x0000FF );
        
        glColor4ub( r, g, b, 0 );
        
        glVertex3f( e.vertex_number( 0 )._x, e.vertex_number( 0 )._y, e.vertex_number( 0 )._z );
        glVertex3f( e.vertex_number( 1 )._x, e.vertex_number( 1 )._y, e.vertex_number( 1 )._z );
        glVertex3f( e.vertex_number( 2 )._x, e.vertex_number( 2 )._y, e.vertex_number( 2 )._z );
        
        if( e.vertex_count() == 4 )
        {
            glVertex3f( e.vertex_number( 2 )._x, e.vertex_number( 2 )._y, e.vertex_number( 2 )._z );
            glVertex3f( e.vertex_number( 3 )._x, e.vertex_number( 3 )._y, e.vertex_number( 3 )._z );
            glVertex3f( e.vertex_number( 0 )._x, e.vertex_number( 0 )._y, e.vertex_number( 0 )._z );
        }
        
    }
    
    glEnd();
}

void radiosity :: draw_elements()
{   
    for( unsigned index = 0; index < _elements.size(); ++index )
    {
        element &e = _elements[ index ];

        if( e.vertex_count() == 3 )
        {
            glBegin( GL_TRIANGLES );
            glVertex3f( e.vertex_number( 0 )._x, e.vertex_number( 0 )._y, e.vertex_number( 0 )._z );
            glVertex3f( e.vertex_number( 1 )._x, e.vertex_number( 1 )._y, e.vertex_number( 1 )._z );
            glVertex3f( e.vertex_number( 2 )._x, e.vertex_number( 2 )._y, e.vertex_number( 2 )._z );
            glEnd();
        }
        else
        {
            glBegin( GL_QUADS );
            glVertex3f( e.vertex_number( 0 )._x, e.vertex_number( 0 )._y, e.vertex_number( 0 )._z );
            glVertex3f( e.vertex_number( 1 )._x, e.vertex_number( 1 )._y, e.vertex_number( 1 )._z );
            glVertex3f( e.vertex_number( 2 )._x, e.vertex_number( 2 )._y, e.vertex_number( 2 )._z );
            glVertex3f( e.vertex_number( 3 )._x, e.vertex_number( 3 )._y, e.vertex_number( 3 )._z );
            glEnd();
        }
        
    }
}

void radiosity :: draw_patches()
{
    
    for( unsigned index = 0; index < _patches.size(); ++index )
    {
        element &e = _patches[ index ];

        if( e.vertex_count() == 3 )
        {
            glBegin( GL_TRIANGLES );
            glVertex3f( e.vertex_number( 0 )._x, e.vertex_number( 0 )._y, e.vertex_number( 0 )._z );
            glVertex3f( e.vertex_number( 1 )._x, e.vertex_number( 1 )._y, e.vertex_number( 1 )._z );
            glVertex3f( e.vertex_number( 2 )._x, e.vertex_number( 2 )._y, e.vertex_number( 2 )._z );
            glEnd();
        }
        else
        {
            glBegin( GL_QUADS );
            glVertex3f( e.vertex_number( 0 )._x, e.vertex_number( 0 )._y, e.vertex_number( 0 )._z );
            glVertex3f( e.vertex_number( 1 )._x, e.vertex_number( 1 )._y, e.vertex_number( 1 )._z );
            glVertex3f( e.vertex_number( 2 )._x, e.vertex_number( 2 )._y, e.vertex_number( 2 )._z );
            glVertex3f( e.vertex_number( 3 )._x, e.vertex_number( 3 )._y, e.vertex_number( 3 )._z );
            glEnd();
        }
        
    }
}

void radiosity :: calculate_vertex_exitance()
{
    // Clear all of the reflectance values from the vertices.
    for( unsigned index = 0; index < _vertices.size(); ++index )
    {
        _vertices[ index ]._reflectance = color( 0, 0, 0 );
        _vertices[ index ]._reflectance_count = 0;
    }

    // Loop through every element and add the element's exitance to the element's vertices.
    for( unsigned index = 0; index < _elements.size(); ++index )
    {
        element &e = _elements[ index ];
        
        for( unsigned vert = 0; vert < e.vertex_count(); ++vert )
        {
            e.vertex_number( vert )._reflectance._red += _exitances[ index ]._red + _emittance[ index ]._red;
            e.vertex_number( vert )._reflectance._green += _exitances[ index ]._green + _emittance[ index ]._green;
            e.vertex_number( vert )._reflectance._blue += _exitances[ index ]._blue + _emittance[ index ]._blue;
            e.vertex_number( vert )._reflectance_count++;
        }
    }

    // Set the vertex's reflectance to the average of all of the elements that share it (since they
    // were summed above.)
    for( unsigned index = 0; index < _vertices.size(); ++index )
    {
        _vertices[ index ]._reflectance /= (float)_vertices[ index ]._reflectance_count;
    }
}

void radiosity :: draw_full()
{
    calculate_vertex_exitance();
    
    for( unsigned index = 0; index < _elements.size(); ++index )
    {
        element &e = _elements[ index ];
        if( e.vertex_count() == 3 )
        {
            glBegin( GL_TRIANGLES );
            glColor3f( e.vertex_number( 0 )._reflectance._red, e.vertex_number( 0 )._reflectance._green, e.vertex_number( 0 )._reflectance._blue );
            glVertex3f( e.vertex_number( 0 )._x, e.vertex_number( 0 )._y, e.vertex_number( 0 )._z );
            glColor3f( e.vertex_number( 1 )._reflectance._red, e.vertex_number( 1 )._reflectance._green, e.vertex_number( 1 )._reflectance._blue );
            glVertex3f( e.vertex_number( 1 )._x, e.vertex_number( 1 )._y, e.vertex_number( 1 )._z );
            glColor3f( e.vertex_number( 2 )._reflectance._red, e.vertex_number( 2 )._reflectance._green, e.vertex_number( 2 )._reflectance._blue );
            glVertex3f( e.vertex_number( 2 )._x, e.vertex_number( 2 )._y, e.vertex_number( 2 )._z );
            glEnd();
        }
        else
        {
            glBegin( GL_QUADS );
            glColor3f( e.vertex_number( 0 )._reflectance._red, e.vertex_number( 0 )._reflectance._green, e.vertex_number( 0 )._reflectance._blue );
            glVertex3f( e.vertex_number( 0 )._x, e.vertex_number( 0 )._y, e.vertex_number( 0 )._z );
            glColor3f( e.vertex_number( 1 )._reflectance._red, e.vertex_number( 1 )._reflectance._green, e.vertex_number( 1 )._reflectance._blue );
            glVertex3f( e.vertex_number( 1 )._x, e.vertex_number( 1 )._y, e.vertex_number( 1 )._z );
            glColor3f( e.vertex_number( 2 )._reflectance._red, e.vertex_number( 2 )._reflectance._green, e.vertex_number( 2 )._reflectance._blue );
            glVertex3f( e.vertex_number( 2 )._x, e.vertex_number( 2 )._y, e.vertex_number( 2 )._z );
            glColor3f( e.vertex_number( 3 )._reflectance._red, e.vertex_number( 3 )._reflectance._green, e.vertex_number( 3 )._reflectance._blue );
            glVertex3f( e.vertex_number( 3 )._x, e.vertex_number( 3 )._y, e.vertex_number( 3 )._z );
            glEnd();
        }
        
    }
}

void radiosity :: cornell_box()
{
    _vertices.resize( 0 );
    _patches.resize( 0 );
    
    int vert_index = 0;
    
    // light
    _vertices.push_back( vertex( 0, 343.0, 547.7, 227.0 ) ); 
    _vertices.push_back( vertex( 0, 343.0, 547.7, 332.0 ) );
    _vertices.push_back( vertex( 0, 213.0, 547.7, 332.0 ) );
    _vertices.push_back( vertex( 0, 213.0, 547.7, 227.0 ) );
    _patches.push_back( element( color( 1, 1, 1 ), &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ] ) );
   
    // floor
    _vertices.push_back( vertex( 1, 552.8, 0.0, 0.0 ) );
    _vertices.push_back( vertex( 1, 0.0, 0.0, 0.0 ) );
    _vertices.push_back( vertex( 1, 0.0, 0.0, 559.2 ) );
    _vertices.push_back( vertex( 1, 549.6, 0.0, 559.2 ) );
    _patches.push_back( element( color( 1, 1, 1 ), &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ] ) );
    
    // ceiling
    _vertices.push_back( vertex( 2, 556.0, 548.8, 0.0 ) );   
    _vertices.push_back( vertex( 2, 556.0, 548.8, 559.2 ) );
    _vertices.push_back( vertex( 2, 0.0, 548.8, 559.2 ) );
    _vertices.push_back( vertex( 2, 0.0, 548.8, 0.0 ) );
    _patches.push_back( element( color( 1, 1, 1 ), &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ] ) );
    
    // back wall (white)
    _vertices.push_back( vertex( 3, 549.6, 0.0, 559.2 ) );
    _vertices.push_back( vertex( 3, 0.0, 0.0, 559.2 ) );
    _vertices.push_back( vertex( 3, 0.0, 548.8, 559.2 ) );
    _vertices.push_back( vertex( 3, 556.0, 548.8, 559.2 ) );
    _patches.push_back( element( color( 1, 1, 1 ), &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ] ) );

    // right wall (green)
    _vertices.push_back( vertex( 4, 0.0, 0.0, 559.2 ) );   
    _vertices.push_back( vertex( 4, 0.0, 0.0, 0.0 ) );
    _vertices.push_back( vertex( 4, 0.0, 548.8, 0.0 ) );
    _vertices.push_back( vertex( 4, 0.0, 548.8, 559.2 ) );
    _patches.push_back( element( color( 0, 1, 0 ), &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ] ) );

    // left wall (red)
    _vertices.push_back( vertex( 5, 552.8, 0.0, 0.0 ) ); 
    _vertices.push_back( vertex( 5, 549.6, 0.0, 559.2 ) );
    _vertices.push_back( vertex( 5, 556.0, 548.8, 559.2 ) );
    _vertices.push_back( vertex( 5, 556.0, 548.8, 0.0 ) );
    _patches.push_back( element( color( 1, 0, 0 ), &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ] ) );

    // short block (white)
    _vertices.push_back( vertex( 6, 130.0, 165.0,  65.0 ) ); 
    _vertices.push_back( vertex( 6,  82.0, 165.0, 225.0 ) ); 
    _vertices.push_back( vertex( 6, 240.0, 165.0, 272.0 ) ); 
    _vertices.push_back( vertex( 6, 290.0, 165.0, 114.0 ) ); 
    _patches.push_back( element( color( 1, 1, 1 ), &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ] ) );

    _vertices.push_back( vertex( 7, 290.0,   0.0, 114.0 ) ); 
    _vertices.push_back( vertex( 7, 290.0, 165.0, 114.0 ) ); 
    _vertices.push_back( vertex( 7, 240.0, 165.0, 272.0 ) ); 
    _vertices.push_back( vertex( 7, 240.0,   0.0, 272.0 ) ); 
    _patches.push_back( element( color( 1, 1, 1 ), &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ] ) );

    _vertices.push_back( vertex( 8, 130.0,   0.0,  65.0 ) ); 
    _vertices.push_back( vertex( 8, 130.0, 165.0,  65.0 ) ); 
    _vertices.push_back( vertex( 8, 290.0, 165.0, 114.0 ) ); 
    _vertices.push_back( vertex( 8, 290.0,   0.0, 114.0 ) ); 
    _patches.push_back( element( color( 1, 1, 1 ), &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ] ) );

    _vertices.push_back( vertex( 9,  82.0,   0.0, 225.0 ) ); 
    _vertices.push_back( vertex( 9,  82.0, 165.0, 225.0 ) ); 
    _vertices.push_back( vertex( 9, 130.0, 165.0,  65.0 ) ); 
    _vertices.push_back( vertex( 9, 130.0,   0.0,  65.0 ) ); 
    _patches.push_back( element( color( 1, 1, 1 ), &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ] ) );

    _vertices.push_back( vertex( 10, 240.0,   0.0, 272.0 ) ); 
    _vertices.push_back( vertex( 10, 240.0, 165.0, 272.0 ) ); 
    _vertices.push_back( vertex( 10,  82.0, 165.0, 225.0 ) ); 
    _vertices.push_back( vertex( 10,  82.0,   0.0, 225.0 ) ); 
    _patches.push_back( element( color( 1, 1, 1 ), &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ] ) );


    // tall block (white)
    _vertices.push_back( vertex( 11,423.0, 330.0, 247.0 ) ); 
    _vertices.push_back( vertex( 11,265.0, 330.0, 296.0 ) ); 
    _vertices.push_back( vertex( 11,314.0, 330.0, 456.0 ) ); 
    _vertices.push_back( vertex( 11,472.0, 330.0, 406.0 ) ); 
    _patches.push_back( element( color( 1, 1, 1 ), &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ] ) );

    _vertices.push_back( vertex( 12,423.0,   0.0, 247.0 ) ); 
    _vertices.push_back( vertex( 12,423.0, 330.0, 247.0 ) ); 
    _vertices.push_back( vertex( 12,472.0, 330.0, 406.0 ) ); 
    _vertices.push_back( vertex( 12,472.0,   0.0, 406.0 ) ); 
    _patches.push_back( element( color( 1, 1, 1 ), &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ] ) );

    _vertices.push_back( vertex( 13, 472.0,   0.0, 406.0 ) ); 
    _vertices.push_back( vertex( 13, 472.0, 330.0, 406.0 ) ); 
    _vertices.push_back( vertex( 13, 314.0, 330.0, 456.0 ) ); 
    _vertices.push_back( vertex( 13, 314.0,   0.0, 456.0 ) ); 
    _patches.push_back( element( color( 1, 1, 1 ), &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ] ) );

    _vertices.push_back( vertex( 14,314.0,   0.0, 456.0 ) ); 
    _vertices.push_back( vertex( 14,314.0, 330.0, 456.0 ) ); 
    _vertices.push_back( vertex( 14,265.0, 330.0, 296.0 ) ); 
    _vertices.push_back( vertex( 14,265.0,   0.0, 296.0 ) ); 
    _patches.push_back( element( color( 1, 1, 1 ), &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ] ) );

    _vertices.push_back( vertex( 15,265.0,   0.0, 296.0 ) ); 
    _vertices.push_back( vertex( 15,265.0, 330.0, 296.0 ) ); 
    _vertices.push_back( vertex( 15,423.0, 330.0, 247.0 ) ); 
    _vertices.push_back( vertex( 15,423.0,   0.0, 247.0 ) ); 
    _patches.push_back( element( color( 1, 1, 1 ), &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ], &_vertices[ vert_index++ ] ) );

    _patch_exitances.resize( _patches.size() );
    _emittance.resize( _patches.size() );
    
    _emittance[ 0 ]._red = 32.0f;
    _emittance[ 0 ]._green = 32.0f;
    _emittance[ 0 ]._blue = 32.0f;

    _patch_exitances[ 0 ] = _emittance[ 0 ];
       
    for( unsigned index = 1; index < _patch_exitances.size(); ++index )
    {
        _emittance[ index ] = color( 0, 0, 0 );
        _patch_exitances[ index ] = color( 0, 0, 0 );
    }
}

void radiosity :: create_elements()
{
    _elements.resize( 0 );
    _exitances.resize( 0 );
    _element_owners.resize( 0 );
    _form_factors.resize( 0 );
    
    for( unsigned index = 0; index < _patches.size(); ++index )
    {
        _elements.push_back( _patches[ index ] );
        _exitances.push_back( color( 0, 0, 0 ) );
        _element_owners.push_back( index );
        _form_factors.push_back( 0.0f );
    }
   
}

void radiosity :: subdivide_elements( unsigned count )
{
    while( count-- )
    {
        size_t element_count = _elements.size(); // this must be precomputed because subdividing will add more
        for( size_t index = 0; index < element_count; ++index )
        {
            element &e = _elements[ index ];
        
            e.subdivide( _elements, _vertices );
            _exitances.push_back( _exitances[ index ] );
            _element_owners.push_back( _element_owners[ index ] );
            _form_factors.push_back( 0.0 );
            _emittance.push_back( _emittance[ index ] );
        }
    }
}

void radiosity :: subdivide_patches( unsigned count )
{
    while( count-- )
    {
        size_t patch_count = _patches.size(); // this must be precomputed because subdividing will add more
        for( size_t index = 0; index < patch_count; ++index )
        {
            element &e = _patches[ index ];
        
            e.subdivide( _patches, _vertices );
            _emittance.push_back( _emittance[ index ] );
            _patch_exitances.push_back( _patch_exitances[ index ] );
        }
    }
}

unsigned radiosity :: steps() const
{
    return _steps;
}

unsigned radiosity :: determine_patch_with_highest_unsent_flux(float &total_unsent_flux) const
{
    //
    // walk the element list and add all their unsent flux values, also
    // remember the element with the highest unsent flux, we'll render from
    // that element's point of view on this iteration
    //
    float highest_unsent_flux = 0;
    
    unsigned index_of_patch_with_highest_unsent_flux = 0;

    total_unsent_flux = 0.0f;
    for( unsigned index = 0; index < _patches.size(); ++index )
    {
        const float unsent_flux = calculate_patch_unsent_flux(index);
        total_unsent_flux += unsent_flux;
        
        if( unsent_flux > highest_unsent_flux )
        {
            highest_unsent_flux = unsent_flux;
            index_of_patch_with_highest_unsent_flux = index;
        }
    }

    return index_of_patch_with_highest_unsent_flux;
}
    
float radiosity :: calculate_patch_unsent_flux( unsigned patch_index ) const
{
    return _patches[ patch_index ].area() * ( _patch_exitances[ patch_index ]._red + _patch_exitances[ patch_index ]._green + _patch_exitances[ patch_index ]._blue );
}

