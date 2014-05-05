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
