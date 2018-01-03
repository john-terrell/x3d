#pragma once

#include <vector>
#include "opengl.h"

class mesh
{
public:
    struct vertex 
    {
        float _x, _y, _z;

        vertex(float x, float y, float z) :
            _x(x),
            _y(y),
            _z(z)
        {
        }
    };

    void addVertex(float x, float y, float z)
    {
        _vertices.push_back(vertex(x, y, z));
    }

    void addIndex(GLint index)
    {
        _indices.push_back(index);
    }

    struct primitive
    {
        GLenum _mode;   // GL_TRIANGLES, etc
        GLint _first;   // first index
        GLsizei _count; // number of primitives

        primitive(GLenum mode, GLint first, GLsizei count) :
            _mode(mode),
            _first(first),
            _count(count)
        {
        }
    };

    void addPrimitive(GLenum mode, GLint first, GLsizei count)
    {
        _primitives.push_back(primitive(mode, first, count));
    }

private:
    std::vector<vertex> _vertices;
    std::vector<GLint> _indices;
    std::vector<primitive> _primitives;
};
