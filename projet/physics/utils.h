#ifndef BTUTILS_H
#define BTUTILS_H

#include <QString>
#include "btBulletCollisionCommon.h"
#include "utils/maths.h"
#include <QDebug>

//! Utility functions
/*!
        This file contains several functions used for calculation purposes : physical calculations of energies, ect
        It also contains usefull plain data structures for simulations
    */
float rot_x(float x , float y , float z , float w);
float rot_y(float x , float y , float z , float w);
float rot_z(float x , float y , float z , float w);

btVector3 btQuat2euler(btQuaternion q);

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
        btVector3 _rotation;
        btVector3 _center_of_mass_world_speed;
        btVector3 _angular_speed;
        btVector3 _rotation_vector_diff;
        state_data():_center_of_mass_world_position(btScalar(0),btScalar(0),btScalar(0)),
            _rotation(btScalar(0),btScalar(0),btScalar(0)),
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

#endif // BTUTILS_H
