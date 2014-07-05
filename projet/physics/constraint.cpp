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
#include "constraint.h"

Constraint::Constraint():
    _constraint(NULL),
    _type(point),
    _first_base(false),
    _second_base(false)
{
    _parts.first = NULL;
    _parts.second= NULL;
}

Constraint::Constraint(constraints_types type, Part * first, Part * second, bool first_base, bool second_base):
    _constraint(NULL),
    _type(type),
    _first_base(first_base),
    _second_base(second_base)
{
    _parts.first = first;
    _parts.second= second;
}

Constraint::Constraint(const Constraint& other):
    _constraint(NULL),
    _type(other._type),
    _first_base(other._first_base),
    _second_base(other._second_base)
{
    _parts.first = other._parts.first;
    _parts.second = other._parts.second;
}

Constraint& Constraint::operator=( const Constraint& other ) {
    _constraint = NULL;
    _type=other._type;
    _parts.first = other._parts.first;
    _parts.second = other._parts.second;
    return *this;
}

Constraint::~Constraint(){
    deleteConstraint();
}

void Constraint::deleteConstraint(){
    if (_constraint != NULL){
        delete _constraint;
        _constraint = NULL;
    }
}

bool Constraint::buildConstraint(){
    if (has_parts()){
        allocateConstraint();
        return true;
    } else return false;
}

void Constraint::allocateConstraint(){
    btVector3 pivotA, pivotB;
    btTransform localeA, localeB;
    btVector3 shapeA, shapeB  ;
    btRigidBody *bodyA,*bodyB;
    btQuaternion rotationA,rotationB;
    btVector3 rotA,rotB;

    btTypedConstraint * constraint;

    shapeA = _parts.first->get_shape_struct().get_shape();
    bodyA = _parts.first->get_body();

    if (_parts.second != NULL){
        shapeB = _parts.second->get_shape_struct().get_shape();
        bodyB = _parts.second->get_body();
    }
    rotationA.setEuler(0,0,M_PI_2);
    rotationB.setEuler(deg2rad(rotA.y()), deg2rad(rotA.x()), deg2rad(rotA.z()+90));

    //setting the position of the pivot points. The posision is specified in local coordinates
    if (_first_base)  pivotA =   btVector3(0,shapeA.y()/2,0);
    else              pivotA = - btVector3(0,shapeA.y()/2,0);
    if (_second_base) pivotB =   btVector3(0,shapeB.y()/2,0);
    else              pivotB = - btVector3(0,shapeB.y()/2,0);

    //localeA and localeB are not used for point2pointConstraints
    localeA.setIdentity();localeA.setRotation(rotationA);localeA.setOrigin(pivotA);
    localeB.setIdentity();localeB.setRotation(rotationB);localeB.setOrigin(pivotB);

    deleteConstraint();
    switch (_type){
    case point :
        if (_parts.second != NULL) constraint= new btPoint2PointConstraint(*bodyA,*bodyB,pivotA,pivotB);
        else                       constraint= new btPoint2PointConstraint(*bodyA,pivotA);
        break;
    case cone:
        if (_parts.second != NULL){
            constraint= new btConeTwistConstraint(*bodyA,*bodyB,localeA,localeB);
            ((btConeTwistConstraint *)constraint)->setLimit(M_PI,M_PI,0);
        }
        else qWarning()<<"cannot create cone constraint on one object";
        break;
    case hinge:
        if (_parts.second != NULL){
            constraint= new btHingeConstraint(*bodyA,*bodyB,localeA,localeB);
            ((btHingeConstraint*)constraint)->setLimit(btScalar(-M_PI_4), btScalar(M_PI_2));
        }
        else qWarning()<<"cannot create hinge constraint on one object";
        break;
    default:
        break;
    }
    _constraint = (btTypedConstraint*) constraint;
}
