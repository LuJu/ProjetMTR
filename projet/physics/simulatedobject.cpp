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
#include "simulatedobject.h"


SimulatedObject::SimulatedObject():
    _body(NULL),
    _motion_state(NULL),
    _local_inertia(btVector3(0,0,0)),
    _collision_shape(NULL),
    _shape(NULL)
{}

btRigidBody *  SimulatedObject::get_body(btScalar mass,btTransform transform, btScalar com_proportion){
    if(!_body || !_motion_state){
        buildMotion(mass,transform,com_proportion);
    }
    return _body;
}

void SimulatedObject::deleteMotion(){
    if(_body != NULL){delete _body; _body = NULL;}
    if(_motion_state != NULL){delete _motion_state; _motion_state = NULL;}
}

void  SimulatedObject::buildMotion(btScalar mass,btTransform transform, btScalar com_proportion){
    deleteMotion();
    _collision_shape = _shape->newCollisionShape();
    _collision_shape->calculateLocalInertia(mass,_local_inertia);
    btScalar length = _shape->get_shape().y();
    btScalar com_y_position = length * com_proportion - length/2.0f;
    btTransform com_offset; com_offset.setIdentity();com_offset.setOrigin(btVector3(0,com_y_position,com_y_position));
    _motion_state = new btDefaultMotionState(transform,com_offset);
    btRigidBody::btRigidBodyConstructionInfo construction_info(mass, _motion_state, _collision_shape , _local_inertia);
    _body = new btRigidBody(construction_info);
}
