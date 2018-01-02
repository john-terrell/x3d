#pragma once

#include <math.h>
#include <float.h>
#include <memory.h>
#include <stdexcept>
#include <iostream>
#include <stdlib.h>

#include <cmath>

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
