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
#ifndef SIMULATEDOBJECT_H
#define SIMULATEDOBJECT_H

#include "utils.h"
#include "shape.h"

//! An object simulated with bullet
/*!
        Represents an object "simulated" with bullet. The data of the object has to be saved in the _previous_state and _current_state objects every possible time in order to ensure
        that the calculations work
    */
class SimulatedObject
{
public:
    SimulatedObject();

    btRigidBody * get_body() const;
    btDefaultMotionState * get_motion_state() const ;

    //! Deletes the motion state and the body objects
    void deleteMotion();
    //! Allocates the motion state and the body. Has to be called at least once before calling get_body() or get_motion_state()
    void buildMotion(btScalar mass, btTransform transform, const Shape& shape, btScalar com_proportion=.5f);


private:

    btCollisionShape *_collision_shape;
    btDefaultMotionState * _motion_state;
    btVector3 _local_inertia;
    btRigidBody * _body;
};

#endif // SIMULATEDOBJECT_H
