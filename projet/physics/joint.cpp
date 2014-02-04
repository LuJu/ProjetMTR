#include "joint.h"

Joint::Joint():
    _constraint(NULL)
{
    _parts.first = NULL;
    _parts.second= NULL;
}
