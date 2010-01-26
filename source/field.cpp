#include <x3d/x3d.h>
#include <x3d/field.h>

using namespace x3d;

class connector_count_validator
{
public:
    ~connector_count_validator()
    {
        assert( abstract_field::connector_count == 0 );
    }
};

static connector_count_validator ccv;   // this object will validate that all of our
                                        // connectors are destroyed (not being leaked)

unsigned long abstract_field::connector_count = 0;
