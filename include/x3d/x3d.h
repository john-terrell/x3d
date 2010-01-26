#ifndef x3d__x3d_h
#define x3d__x3d_h

#include <math.h>
#include <float.h>
#include <memory.h>
#include <stdexcept>
#include <iostream>
#include <stdlib.h>

#include <x3d/ieee_fp.h>

#ifndef NDEBUG
#ifndef assert
void x3d_assertion_failed( const char *data, const char *file, unsigned line );
#define assert(x) if( !(x) ) x3d_assertion_failed( #x, __FILE__, __LINE__ );
#endif
#else
#ifndef assert
#define assert ;
#endif
#endif

namespace x3d {}

#endif // x3d__x3d_h
