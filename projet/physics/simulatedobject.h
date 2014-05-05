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

    state_information _information;
    btCollisionShape *_collision_shape;

    btRigidBody * get_body(btScalar mass,btTransform transform, btScalar com_proportion=.5f);
    void deleteMotion();
    void  buildMotion(btScalar mass,btTransform transform, btScalar com_proportion=.5f);

    void set_shape(Shape * shape){
        _shape = shape;
    }

    btDefaultMotionState * get_motion_state() const {
        if(!_motion_state)
            qWarning()<<"Motion state not defined";
        return _motion_state;
    }

private:

    Shape * _shape;
    btDefaultMotionState * _motion_state;
    btVector3 _local_inertia;
    btRigidBody * _body;
};

#endif // SIMULATEDOBJECT_H
