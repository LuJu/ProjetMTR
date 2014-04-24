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
//float rot_x(btQuaternion q);
//float rot_y(btQuaternion q);
//float rot_z(btQuaternion q);
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

//void initialize_simple_point(t_simple_point * point){
//    point->x = 0.0f;
//    point->y = 0.0f;
//    point->z = 0.0f;
//}

//void initialize_energy_info(t_energy_info * info){

//}

//initialize_part_info(t_part_info * part){
//    part->ake_diff = 0.0f;
//}

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

btScalar get_moment(btVector3 rotation_axis,btVector3 shape, btScalar mass);
double kinetic_energy(double speed, double mass);
double potential_energy(double mass, double gravitation, double height);
void toString(const btVector3& vector, const QString& prefix);
void toString(const btQuaternion& quaternion,const QString& prefix);

#endif // BTUTILS_H
