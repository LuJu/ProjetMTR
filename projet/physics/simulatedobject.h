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
    SimulatedObject():
        _body(NULL),
        _motion_state(NULL),
        _local_inertia(btVector3(0,0,0)),
        _collision_shape(NULL)
    {}

    state_data _previous_state,_current_state;



    btDefaultMotionState * _motion_state;
    btVector3 _local_inertia;
    btRigidBody * _body;
    btCollisionShape *_collision_shape;

    Shape * _shape;

    btRigidBody &  get_body(btScalar mass,btTransform transform){
        if(!_body || !_motion_state){
            buildMotion(mass,transform);
        }
        return *_body;
    }

    void deleteMotion(){
        if(_body != NULL){
            delete _body; _body = NULL;}
        if(_motion_state != NULL){
            delete _motion_state; _motion_state = NULL;}
//        if(_collision_shape != NULL) {
//            _shape.deleteCollisionShape(_collision_shape); _collision_shape = NULL;}
    }

    void  buildMotion(btScalar mass,btTransform transform){
        deleteMotion();
        _collision_shape = _shape->newCollisionShape();
        _collision_shape->calculateLocalInertia(mass,_local_inertia);
        _motion_state = new btDefaultMotionState(transform);
        btRigidBody::btRigidBodyConstructionInfo construction_info(mass, _motion_state, _collision_shape , _local_inertia);
        _body = new btRigidBody(construction_info);
    }
};

#endif // SIMULATEDOBJECT_H
