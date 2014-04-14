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
float rot_x(btQuaternion q);
float rot_y(btQuaternion q);
float rot_z(btQuaternion q);

btVector3 btQuat2euler(btQuaternion q);

typedef struct simple_point{
    float x,y,z;
}t_simple_point;

typedef struct energy_info{
    float x,y,z;
    simple_point pt_aspeed;
    simple_point pt_speed;
    float speed;
    float aspeed; //angular speed
    float ake,ke,pe;
}t_energy_info;

typedef struct part_info{
    QString part_name;
    float length;
    t_energy_info animation, simulation;
    float ake_diff;
    float ke_diff;
    float pe_diff;
}t_part_info;

typedef struct state_data{
        btVector3 _position;
        btVector3 _rotation;
        btVector3 _center_of_mass_speed;
        btVector3 _angular_speed;
        btVector3 _rotation_vector_diff;
//        btVector3 _center_of_mass_speed;

}t_state_data;

btScalar get_moment(btVector3 rotation_axis,btVector3 shape, btScalar mass);
double kinetic_energy(double speed, double mass);
double potential_energy(double mass, double gravitation, double height);
void printbtVector3(const QString& prefix,const btVector3& vector);


#endif // BTUTILS_H
