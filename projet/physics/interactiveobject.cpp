#include "interactiveobject.h"

InteractiveObject::InteractiveObject(const btVector3& origin, const btVector3& shape,shapetype type):
    _mass(1),
    _animated(false),
    _body(NULL),
    _motion_state(NULL),
    _local_inertia(btVector3(0,0,0)),
    _shape_type(type),
    _speed_error(0),
    _ticks(0){
    switch(type){
    case cube:
        _shape = new btBoxShape(shape);
        break;
    case cylinder:
        _shape = new btCylinderShape(shape);
        break;
    }
    _transform.setIdentity();
    _transform.setOrigin(origin);

    _animation_from_simulation.get_translation_curves()[0]._color[0]=1;
    _animation_from_simulation.get_translation_curves()[0]._color[1]=0;
    _animation_from_simulation.get_translation_curves()[0]._color[2]=0;

    _animation_from_simulation.get_translation_curves()[1]._color[0]=0;
    _animation_from_simulation.get_translation_curves()[1]._color[1]=1;
    _animation_from_simulation.get_translation_curves()[1]._color[2]=0;

    _animation_from_simulation.get_translation_curves()[2]._color[0]=0;
    _animation_from_simulation.get_translation_curves()[2]._color[1]=0;
    _animation_from_simulation.get_translation_curves()[2]._color[2]=1;
}

//InteractiveObject::InteractiveObject(const InteractiveObject& object) :
//    _animated(false),
//    _mass(object._mass),
//    _transform(object._transform),
//    _local_inertia(object._local_inertia),
//    _body(NULL),
//    _motion_state(NULL){
//    _shape = new btBoxShape(((btBoxShape*)object._shape)->getHalfExtentsWithMargin());
//}

InteractiveObject::~InteractiveObject(){
    if(_shape) delete _shape;
    if(_body)  delete _body;
    if(_motion_state) delete _motion_state;
}

btRigidBody & InteractiveObject::get_body(){
    if(!_body && !_motion_state){
        buildMotion();
    }
    return *_body;
}

void InteractiveObject::deleteMotion(){
    if(_body)  delete _body;
    if(_motion_state) delete _motion_state;
}

void InteractiveObject::buildMotion(){
    deleteMotion();
    _shape->calculateLocalInertia(_mass,_local_inertia);
    _motion_state = new btDefaultMotionState(_transform);
    btRigidBody::btRigidBodyConstructionInfo construction_info(_mass, _motion_state, _shape, _local_inertia);
    _body = new btRigidBody(construction_info);
}

btVector3 InteractiveObject::get_shape() const {
    return ((btBoxShape *) _shape)->getHalfExtentsWithMargin();
}

void InteractiveObject::set_shape(const btVector3 &shape){
    delete _shape;
    _shape = new btBoxShape(shape);
}

const InteractiveObject::t_energy_info& InteractiveObject::update_energy_info(float elapsed,float diff,float gravity){
    btVector3 distance(_animation.translationVector(elapsed)-_previous_position);
    btVector3 simulation_distance = _body->getCenterOfMassPosition()-_previous_position_simulation;
    btVector3 speed(distance/diff);
    _animation_speed = speed*1000;
    _calculated_simulation_speed = simulation_distance/diff;
    _speed_error += absolute_value(_calculated_simulation_speed.length() - _body->getLinearVelocity().length());
    _previous_position = _animation.translationVector(elapsed);

    _energy.animation_speed = speed.length()*1000;
    _energy.simulation_speed = _body->getLinearVelocity().length();

    _energy.animation_ke = kinetic_energy(speed.length()*1000,_mass);
    _energy.simulation_ke = kinetic_energy(_body->getLinearVelocity().length(),_mass);

    _energy.animation_pe = potential_energy(_mass,gravity,_animation.translationVector(elapsed).y());
    _energy.simulation_pe = potential_energy(_mass,gravity,_body->getCenterOfMassPosition().y());

    _energy.animation_ake = 0;
    _energy.simulation_ake = 0;

    _energy.animation_x = _animation.translationVector(elapsed).x();
    _energy.animation_y = _animation.translationVector(elapsed).y();
    _energy.animation_z = _animation.translationVector(elapsed).z();

    _energy.simulation_x = _body->getCenterOfMassPosition().x();
    _energy.simulation_y = _body->getCenterOfMassPosition().y();
    _energy.simulation_z = _body->getCenterOfMassPosition().z();

    _energy.part_name = _body_part;
    _animation_from_simulation.get_translation_curves()[0].insert(elapsed,_body->getCenterOfMassPosition().x());
    _animation_from_simulation.get_translation_curves()[1].insert(elapsed,_body->getCenterOfMassPosition().y());
    _animation_from_simulation.get_translation_curves()[2].insert(elapsed,_body->getCenterOfMassPosition().z());
    ++_ticks;
    _energy.mean_error = get_mean_error();
    return _energy;
}

void InteractiveObject::updateAnimationFromSimulation(float elapsed){
}

void InteractiveObject::setSimulationPosition(float time){

    btVector3 shape,translation,rotation;
    btQuaternion quat;
    btTransform transform;

    transform.setIdentity();

    rotation = _animation.rotationVector(time);
    quat.setEuler(deg2rad(rotation.x()),
                  deg2rad(rotation.y()),
                  deg2rad(rotation.z()));
    transform.setRotation(quat);

    translation = _animation.translationVector(time);
    transform.setOrigin(translation);
    set_transform(transform);
    shape =btVector3(_animation.scalingVector(time));
    set_shape(shape);

    buildMotion();
    if (time != 0) {
        btRigidBody& body = get_body();
        body.setLinearVelocity(_animation_speed);
    }

}
