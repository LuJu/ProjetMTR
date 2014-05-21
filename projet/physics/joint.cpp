#include "joint.h"

Joint::Joint():
    _main_part(NULL)
{
}

Joint::Joint(const Joint& other):
    _main_part(other._main_part)
{
    _part_name=other._part_name;
    _animation=other._animation;
}

Joint& Joint::operator=( const Joint& other ) {
    _main_part =other._main_part;
    _part_name=other._part_name;
    _animation=other._animation;
    return *this;
}

Joint::~Joint(){
}
