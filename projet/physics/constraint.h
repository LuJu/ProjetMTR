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
#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <QPair>

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "part.h"

class Constraint
{
public:
    enum constraints_types{
        hinge,
        point,
        cone
    };
    Constraint();
    //! Constraint constructor
    /*!
        \param  first : The first part to be connected to the constraint
        \param  second : The second part to be connected to the constraint
        \param  first base : Indicates whether the first part is to be connected to its top(true) or its base(false)
        \param  second base : Indicates whether the second part is to be connected to its top(true) or its base(false)
        return output
    */
    Constraint(Part * first, Part * second, bool first_base = false, bool second_base = false);
    Constraint& operator=( const Constraint& other ) ;
    explicit Constraint(const Constraint& other);
    virtual ~Constraint();

    btTypedConstraint* get_constraint() const;
    bool buildConstraint();

    bool is_constraint_allocated(){return _constraint!=NULL;}
    bool has_parts() const;
    QPair<Part*,Part*>& get_parts(){return _parts;}
    void set_constraint_type(constraints_types type){_type = type;}

private:
    QPair<Part* ,Part* > _parts;
    btTypedConstraint * _constraint;
    void deleteConstraint();
    bool _first_base,_second_base;
    void allocateConstraint();
    constraints_types _type;
};

#endif // CONSTRAINT_H


