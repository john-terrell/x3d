#ifndef x3d__radiosity__winged_edge_h
#define x3d__radiosity__winged_edge_h

/*

ccw_left\  /cw_right
         \/
         ^ - head_vertex
         |
  left   |  right
  face   |  face
         |
         | - tail_vertex
         /\
cw_left /  \ ccw_right

*/
namespace x3d
{
    struct w_face
    {
    };

    struct w_vertex
    {
        float _x, _y, _z;
    };

    struct w_edge
    {
        w_face *_left_face, *_right_face;
        
        w_vertex *_head_vertex, *_tail_vertex;
                
        w_edge *_ccw_left_edge, *_cw_right_edge;  // connected to _head_vertex
        w_edge *_cw_left_edge, *_ccw_right_edge;  // connected to _tail_vertex
    };
};

#endif // x3d__radiosity__winged_edge_h
