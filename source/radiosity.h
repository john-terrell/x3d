#pragma once

#include "element.h"
#include "color.h"
#include "hemicube.h"

#include <vector>

//
// unsent_flux = sum of exitance bands * area

class radiosity
{
    std::vector< color > _emittance;            // per element
    std::vector< float > _form_factors;         // per element
    std::vector< color > _exitances;            // per element
    std::vector< color > _patch_exitances;
    std::vector< unsigned > _element_owners;
    std::vector< unsigned > _pixelBuffer;

    float _total_flux, _total_unsent_flux;
    unsigned _index_of_patch_with_highest_unsent_flux;
    
    int _glut_id;
    unsigned _resolution;
    hemicube _hemicube;
    unsigned _steps;
    
public:
    std::vector< element > _patches;
    std::vector< element > _elements;
    std::vector< vertex > _vertices;
    
    size_t vertex_count() const
    {
        return _vertices.size();
    }

    size_t patch_count() const
    {
        return _patches.size();
    }

    size_t element_count() const
    {
        return _elements.size();
    }

public:
    radiosity( int glut_parent_window, unsigned resolution );
    
    void execute_iteration();
    float convergence() const;
    float total_flux() const
    {
        return _total_flux;
    }
    
    float total_unsent_flux() const
    {
        return _total_unsent_flux;
    }
    
    void draw_elements();
    void draw_patches();
    void draw_full();

    void cornell_box();
    void subdivide_patches( unsigned count );
    void subdivide_elements( unsigned count );
    void create_elements();
    
    unsigned steps() const;

    vertex centroid() const 
    {
        auto centroid = vertex();
        for (auto index = 0; index < _vertices.size(); ++index)
        {
            centroid._x += _vertices[index]._x;
            centroid._y += _vertices[index]._y;
            centroid._z += _vertices[index]._z;
        }
        
        centroid._x /= _vertices.size();
        centroid._y /= _vertices.size();
        centroid._z /= _vertices.size();
        
        return centroid;
    }
    
protected:
    float calculate_patch_unsent_flux( unsigned patch_index ) const;
    void calculate_frame_from_element( const element &e, vector3 frame[ 3 ] );
    void calculate_form_factors( unsigned element_index );
    void render( unsigned element_index, const vector3 &view, const vector3 &up );
    unsigned determine_patch_with_highest_unsent_flux(float& total_unsent_flux) const;
    void calculate_vertex_exitance();
};
