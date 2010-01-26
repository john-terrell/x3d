#ifndef x3d__ieee_fp_h
#define x3d__ieee_fp_h

/*
    Single Precision
    The IEEE single precision floating point standard representation requires a 32 bit word, 
    which may be represented as numbered from 0 to 31, right to left (bit 31 is the MSb)
    The first bit is the sign bit, S, the next eight bits are the exponent bits, 'E', and the 
    final 23 bits are the fraction 'F':

      S EEEEEEEE FFFFFFFFFFFFFFFFFFFFFFF
     31 30    22 21                    0

    The value V represented by the word may be determined as follows:


    If E=255 and F is nonzero, then V=NaN ("Not a number") 
    If E=255 and F is zero and S is 1, then V=-Infinity 
    If E=255 and F is zero and S is 0, then V=Infinity 
    If 0<E<255 then V=(-1)**S * 2 ** (E-127) * (1.F) where "1.F" is intended to represent the binary 
        number created by prefixing F with an implicit leading 1 and a binary point. 
    If E=0 and F is nonzero, then V=(-1)**S * 2 ** (-126) * (0.F) These are "unnormalized" values. 
    If E=0 and F is zero and S is 1, then V=-0 
    If E=0 and F is zero and S is 0, then V=0 
    In particular,


      0 00000000 00000000000000000000000 = 0
      1 00000000 00000000000000000000000 = -0

      0 11111111 00000000000000000000000 = Infinity
      1 11111111 00000000000000000000000 = -Infinity

      0 11111111 00000100000000000000000 = NaN
      1 11111111 00100010001001010101010 = NaN

      0 10000000 00000000000000000000000 = +1 * 2**(128-127) * 1.0 = 2
      0 10000001 10100000000000000000000 = +1 * 2**(129-127) * 1.101 = 6.5
      1 10000001 10100000000000000000000 = -1 * 2**(129-127) * 1.101 = -6.5

      0 00000001 00000000000000000000000 = +1 * 2**(1-127) * 1.0 = 2**(-126)
      0 00000000 10000000000000000000000 = +1 * 2**(-126) * 0.1 = 2**(-127) 
      0 00000000 00000000000000000000001 = +1 * 2**(-126) * 
                                           0.00000000000000000000001 = 
                                           2**(-149)  (Smallest positive value)


    Double Precision
    The IEEE double precision floating point standard representation requires a 64 bit word, 
    which may be represented as numbered from 0 to 63, right to left. 
    The first bit is the sign bit, S, the next eleven bits are the exponent bits, 'E', and 
    the final 52 bits are the fraction 'F':

      S EEEEEEEEEEE FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
     63 62       52 51                                                 0

    The value V represented by the word may be determined as follows:


    If E=2047 and F is nonzero, then V=NaN ("Not a number") 
    If E=2047 and F is zero and S is 1, then V=-Infinity 
    If E=2047 and F is zero and S is 0, then V=Infinity 
    If 0<E<2047 then V=(-1)**S * 2 ** (E-1023) * (1.F) where "1.F" is intended to represent the 
        binary number created by prefixing F with an implicit leading 1 and a binary point. 
    If E=0 and F is nonzero, then V=(-1)**S * 2 ** (-1022) * (0.F) These are "unnormalized" values. 
    If E=0 and F is zero and S is 1, then V=-0 
    If E=0 and F is zero and S is 0, then V=0 
*/
//
// is_finite()
//
inline bool is_finite( float f )
{
    unsigned long* p = reinterpret_cast< unsigned long* >( &f );

    return ( ( *p & 0x7F800000 ) == 0x7F800000 ? false : true );
}

//
// nan()
//
inline float nan()
{
    unsigned long p = 0x7F800001;
    return *reinterpret_cast< float* >( &p ); 
}

//
// positive_infinity()
//
inline float positive_infinity()
{
    unsigned long p = 0x7F800000;
    return *reinterpret_cast< float* >( &p ); 
}

//
// negative_infinity()
//
inline float negative_infinity()
{
    unsigned long p = 0xFF800000;
    return *reinterpret_cast< float* >( &p ); 
}

//
// infinity_error
//
class infinity_error : public std::runtime_error
{
public:
    infinity_error( const char *file, unsigned line ) : std::runtime_error( "Infinity detected" )
    {
    }
};

//
// ensure_finite()
//
inline void ensure_finite( const float *a, size_t count, const char *file, unsigned line )
{
    while( count-- )
    {
        if( !is_finite( *a++ ) )
        {
            throw infinity_error( file, line );
        }
    }
}

//
// ensure_finite()
//
inline void ensure_finite( float a, const char *file, unsigned line )
{
    if( !is_finite( a ) )
        throw infinity_error( file, line );
}

#endif // x3d__ieee_fp_h
