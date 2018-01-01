#ifndef radiosity_h
#define radiosity_h

#include <vector>

#include <x3d/radiosity/element.h>
#include <x3d/matrix44.h>
#include <x3d/color.h>
#include <x3d/radiosity/hemicube.h>
using namespace x3d;

//
// unsent_flux = sum of exitance bands * area

class radiosity
{
    std::vector< color > _emittance;
    std::vector< float > _form_factors;
    std::vector< color > _exitances;
    std::vector< color > _patch_exitances;
    std::vector< unsigned > _element_owners;
    
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
    
public:
    radiosity( int glut_parent_window, unsigned resolution );
    void initialize();
    
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
    void calculate_frame_from_element( const element &e, vector3 frame[ 3 ] );
    void calculate_form_factors( unsigned element_index );
    void render( unsigned element_index, const vector3 &view, const vector3 &up );
    void determine_patch_with_most_unsent_flux();
    void calculate_vertex_exitance();
};

#endif
