/*
Copyright (c) 2013, Lucas Juliéron
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef JOINT_H
#define JOINT_H

#include "part.h"
#include "animatednode.h"
#include "constraint.h"

#include "debuggingutils.h"

//! Class representing a joint between two objects
class Joint
{
public:
    explicit Joint();
    explicit Joint(const Joint& other);
    virtual ~Joint();

    Joint& operator=( const Joint& other ) ;
    bool has_parent() const {
        if (_parent_part_name == "root" || _parent_part_name=="") return false;
        return true;
    }

    AnimatedNode& get_animation() {return _animation;}
    QString _part_name;
    QString _parent_part_name;
    Constraint::constraints_types _constraint_type;
    float _part_mass;
    float _part_com_proportion;

    Part * get_main_part(){ return _main_part; }
    void set_main_part(Part * main_part){ _main_part = main_part; }

private:
    AnimatedNode _animation;
    Part * _main_part;
};

#endif // JOINT_H
