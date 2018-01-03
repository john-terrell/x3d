#pragma once

struct color
{
    float _red, _green, _blue;
    
    color( float red = 0.0f, float green = 0.0f, float blue = 0.0f ) :
        _red( red ),
        _green( green ),
        _blue( blue )
    {
        assert(std::isfinite( red ));
        assert(std::isfinite( green ));
        assert(std::isfinite( blue ));
    }
    
    color & operator += ( const color &a )
    {
        _red += a._red;
        _green += a._green;
        _blue += a._blue;
        
        return *this;
    }
    
    color & operator *= ( float a )
    {
        _red *= a;
        _green *= a;
        _blue *= a;
        
        return *this;
    }
    
    color & operator /= ( float a )
    {
        _red /= a;
        _green /= a;
        _blue /= a;
        
        return *this;
    }
    
    color & operator -= ( const color &a )
    {
        _red -= a._red;
        _green -= a._green;
        _blue -= a._blue;
        
        return *this;
    }
};

inline color operator * ( const color &a, float b )
{
    return color( a._red * b, a._green * b, a._blue * b );
}

inline color operator * ( const color &a, const color &b )
{
    return color( a._red * b._red, a._green * b._green, a._blue * b._blue );
}
