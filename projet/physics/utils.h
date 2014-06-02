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
#ifndef BTUTILS_H
#define BTUTILS_H

#include <QString>
#include "btBulletCollisionCommon.h"
#include "utils/maths.h"
#include <QDebug>
#include <QQuaternion>
#include <qmath.h>
#include "debuggingutils.h"

//! Utility functions
/*!
        This file contains several functions used for calculation purposes : physical calculations of energies, ect
        It also contains usefull plain data structures for simulations
    */
btScalar rot_x(btScalar x , btScalar y , btScalar z , btScalar w);
btScalar rot_y(btScalar x , btScalar y , btScalar z , btScalar w);
btScalar rot_z(btScalar x , btScalar y , btScalar z , btScalar w);

btVector3 btQuat2Euler(btQuaternion q);

btVector3 deg2rad(const btVector3& vector);
btVector3 rad2deg(const btVector3& vector);

typedef struct simple_point{
    float x,y,z;
    simple_point():x(0.0f),y(0.0f),z(0.0f){}
}t_simple_point;

typedef struct energy_info{
    simple_point position;
    simple_point pt_aspeed;
    simple_point pt_speed;
    float speed;
    float aspeed; //angular speed
    float ake,ke,pe;
    energy_info():position(),pt_aspeed(),pt_speed(),
        speed(0.0f),aspeed(0.0f),
        ake(0.0f),ke(0.0f),pe(0.0f){}
}t_energy_info;

typedef struct part_info{
    QString part_name;
    float length;
    t_energy_info animation, simulation;
    float ake_diff;
    float ke_diff;
    float pe_diff;
    part_info():part_name(),length(0.0f),animation(),simulation(),ake_diff(0.0f),ke_diff(0.0f),pe_diff(0.0f){}
}t_part_info;


typedef struct state_data{
        btVector3 _center_of_mass_world_position;
        btVector3 _center_of_mass_world_speed;
        btVector3 _angular_speed; // rad.s-1
        btQuaternion _center_of_mass_rotation;
        btVector3 _rotation_vector_diff;
        state_data():_center_of_mass_world_position(btScalar(0),btScalar(0),btScalar(0)),
            _center_of_mass_world_speed(btScalar(0),btScalar(0),btScalar(0)),
            _angular_speed(btScalar(0),btScalar(0),btScalar(0)),
            _rotation_vector_diff(btScalar(0),btScalar(0),btScalar(0)){}
//        btVector3 _center_of_mass_speed;

}t_state_data;

typedef struct state_information{
    state_data _current,_previous;
    state_information():_current(),_previous(){}
}t_state_information;

btScalar kineticMoment(btVector3 rotation_axis,btVector3 shape, btScalar mass);
double translationKineticEnergy(double speed, double mass);
double potentialEnergy(double mass, double gravitation, double height);

btScalar angularKineticEnergy();
btScalar angularKineticEnergy(btVector3 angular_velocity, btVector3 rotation_vector_diff, btVector3 shape , btScalar mass);
//btQuaternion derivated(const btQuaternion& quat);
btVector3 angleNormalize(const btVector3 rot);
void euler2AxisAngle(btVector3 rotation,float * ret);
btVector3 xyz2yxz(btVector3 xyz);
//btVector3 rotatedVector(btVector3 ypr, btVector3 vector);
btVector3 rotatedVector(btQuaternion quat, btVector3 vector);
#endif // BTUTILS_H
