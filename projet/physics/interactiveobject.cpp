#include "interactiveobject.h"

InteractiveObject::InteractiveObject(const btVector3& origin, const btVector3& shape,shapetype type):
    _mass(1),
    _animated(false),
    _body(NULL),
    _motion_state(NULL),
    _local_inertia(btVector3(0,0,0)),
    _shape_type(type),
    _speed_error(0),
    _speed_error_2(0),
    _ticks(0),
    _ticks_2(0){
    __build(origin,shape,type);
}

InteractiveObject::InteractiveObject():
    _mass(1),
    _animated(false),
    _body(NULL),
    _motion_state(NULL),
    _local_inertia(btVector3(0,0,0)),
    _shape_type(cube),
    _speed_error(0),
    _speed_error_2(0),
    _ticks(0),
    _ticks_2(0){
    __build(btVector3(0,0,0),btVector3(1,1,1),cube);
}

void InteractiveObject::__build(const btVector3& origin, const btVector3& shape,shapetype type){
    switch(type){
    case cube:
        _shape = new btBoxShape(shape);
        break;
    case cylinder:
        _shape = new btCylinderShape(shape);
        break;
    case capsule:
        _shape = new btCapsuleShape(shape.y(),shape.x());
        break;
    }
    _transform.setIdentity();
    _transform.setOrigin(origin);

    _animation_from_simulation.get_translation_curves()[0].set_color(QColor(1,0,0));
    _animation_from_simulation.get_translation_curves()[1].set_color(QColor(0,1,0));
    _animation_from_simulation.get_translation_curves()[2].set_color(QColor(0,0,1));

    Curve c=Curve();
    c.set_color(QColor(255,0,0));
    c.set_label("Animation speed");
    _curves.append(c);
    c=Curve();
    c.set_color(QColor(255,255,0));
    c.set_label("Animation KE");
    _curves.append(c);
    c=Curve();
    c.set_color(QColor(255,255,255));
    c.set_label("animation_pe");
    _curves.append(c);
    c=Curve();
    c.set_color(QColor(0,255,0));
    c.set_label("mean_error");
    _curves.append(c);
    c=Curve();
    c.set_color(QColor(0,255,255));
    c.set_label("work");
    _curves.append(c);
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
    switch (_shape_type){
    case cube:
        return ((btBoxShape *) _shape)->getHalfExtentsWithMargin();
        break;
    case cylinder:
        return ((btCylinderShape *) _shape)->getHalfExtentsWithMargin();
        break;
    case capsule:
        btVector3 shape;
        shape.setX(((btCapsuleShape *)_shape)->getHalfHeight());
        shape.setY(((btCapsuleShape *)_shape)->getRadius());
        shape.setZ(((btCapsuleShape *)_shape)->getHalfHeight());
        return shape;
        break;
    }
}

void InteractiveObject::set_shape(const btVector3 &shape){
    delete _shape;
    switch (_shape_type){
    case cube:
        _shape = new btBoxShape(shape);
        break;
    case cylinder:
        _shape = new btCylinderShape(shape);
        break;
    case capsule:
        _shape = new btCapsuleShape(shape.y(),shape.x());
        break;
    }

}

const InteractiveObject::t_part_info& InteractiveObject::updatePartInfo(float elapsed,float diff,float gravity){
    btVector3 distance(_animation.translationVector(elapsed)-_previous_position);
    btVector3 simulation_distance = _body->getCenterOfMassPosition()-_previous_position_simulation;
    btVector3 speed_animation(distance/(diff/1000)); // the diff value is in ms so a conversion is needed to be in m/s
    _animation_speed = speed_animation;


/// Calculated simulation speed
    _calculated_simulation_speed = simulation_distance/(diff/1000);
    _speed_error += absolute_value(_calculated_simulation_speed.length() - _body->getLinearVelocity().length());
    _previous_position = _animation.translationVector(elapsed);
    _previous_position_simulation_2 = _previous_position_simulation;
    _previous_position_simulation = _body->getCenterOfMassPosition();

    btVector3 simulation_distance_2 = _body->getCenterOfMassPosition()-_previous_position_simulation_2;
    btVector3 _calculated_simulation_speed_2 = simulation_distance_2 / (2*diff/1000);
    _speed_error_2 += absolute_value(_calculated_simulation_speed_2.length() - _previous_linear_velocity.length());
    _previous_linear_velocity = _body->getLinearVelocity();
///

// There are 3 ways of getting the (simulation) speed of the part:
// 1 - getting it from the getLinearVelocity() method of the body class
// 2 - calculating it with the formula s = ( current_position - previous position ) / distance
// 3 - formula s(-1) = ( current_position - previous previous position) / (distance*2)
//    btVector3 speed_simulation =_body->getLinearVelocity(); // first method
    btVector3 speed_simulation =_calculated_simulation_speed; // second method
//    btVector3 speed_simulation =_calculated_simulation_speed_2; // third method

    _energy.animation.speed = speed_animation.length();
    _energy.simulation.speed = speed_simulation.length();

    _energy.animation.ke  = kinetic_energy( speed_animation.length(),                    _mass);
    _energy.simulation.ke = kinetic_energy( speed_simulation.length(),_mass);

    _energy.animation.pe  = potential_energy(_mass,-gravity,_animation.translationVector(elapsed).y());
    _energy.simulation.pe = potential_energy(_mass,-gravity,_body->getCenterOfMassPosition().y()     );

    _energy.animation.ake = 0;
    _energy.simulation.ake = 0;

    _energy.animation.x = _animation.translationVector(elapsed).x();
    _energy.animation.y = _animation.translationVector(elapsed).y();
    _energy.animation.z = _animation.translationVector(elapsed).z();

    _energy.simulation.x = _body->getCenterOfMassPosition().x();
    _energy.simulation.y = _body->getCenterOfMassPosition().y();
    _energy.simulation.z = _body->getCenterOfMassPosition().z();

    _energy.part_name = _body_part;
    _animation_from_simulation.get_translation_curves()[0].insert(elapsed,_body->getCenterOfMassPosition().x());
    _animation_from_simulation.get_translation_curves()[1].insert(elapsed,_body->getCenterOfMassPosition().y());
    _animation_from_simulation.get_translation_curves()[2].insert(elapsed,_body->getCenterOfMassPosition().z());


    ++_ticks;
    ++_ticks_2;
    _energy.mean_error = get_mean_error();
    _energy.mean_error_2 = get_mean_error_2();

    _curves[0].insert(elapsed,_energy.simulation.speed);
    _curves[1].insert(elapsed,_energy.animation.ke);
    _curves[2].insert(elapsed,_energy.animation.pe);
    _curves[3].insert(elapsed,_energy.mean_error);
    _curves[4].insert(elapsed,_energy.work);
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
        _previous_linear_velocity = _animation_speed;
    }

    _previous_position = translation;
    _previous_position_simulation = translation;
    _previous_position_simulation_2 = translation;
//    get_body().setLinearVelocity(btVector3(0,9,0));

}
