#include "interactiveobject.h"

InteractiveObject::InteractiveObject(const btVector3& origin, const btVector3& shape,shapetype type):
    _mass(1),
    _animated(false),
    _body(NULL),
    _motion_state(NULL),
    _local_inertia(btVector3(0,0,0)),
    _shape_type(type){
    __build(origin,shape,type);
}

InteractiveObject::InteractiveObject():
    _mass(1),
    _animated(false),
    _body(NULL),
    _motion_state(NULL),
    _local_inertia(btVector3(0,0,0)),
    _shape_type(cube){
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
        _shape = new btCapsuleShape(shape.x(),shape.y());
        break;
    }
    _transform.setIdentity();
    _transform.setOrigin(origin);

    _animation_from_simulation.get_translation_curves()[0].set_color(QColor(1,0,0));
    _animation_from_simulation.get_translation_curves()[1].set_color(QColor(0,1,0));
    _animation_from_simulation.get_translation_curves()[2].set_color(QColor(0,0,1));

    Curve c=Curve();
    c.set_color(QColor(255,0,0));
    c.set_label("Animation KE");
    _curves.append(c);
    c=Curve();
    c.set_color(QColor(0,0,255));
    c.set_label("animation ake");
    _curves.append(c);
    c=Curve();
    c.set_color(QColor(255,255,255));
    c.set_label("animation_pe");
    _curves.append(c);

    c=Curve();
    c.set_color(QColor(255,0,0));
    c.set_label("Simulation KE");
    _curves.append(c);
    c=Curve();
    c.set_color(QColor(0,0,255));
    c.set_label("Simulation ake");
    _curves.append(c);
    c=Curve();
    c.set_color(QColor(255,255,255));
    c.set_label("Simulation pe");
    _curves.append(c);

    c=Curve();
    c.set_color(QColor(255,0,255));
    c.set_label("ke diff");
    _curves.append(c);
    c=Curve();
    c.set_color(QColor(0,255,255)); // cyan
    c.set_label("ake diff");
    _curves.append(c);
    c=Curve();
    c.set_color(QColor(255,255,0)); // yellow
    c.set_label("pe diff");
    _curves.append(c);

    c=Curve();
    c.set_color(QColor(255,0,0));
    c.set_label("Animation KE");
    _curves_steps.append(c);
    c=Curve();
    c.set_color(QColor(0,0,255));
    c.set_label("animation ake");
    _curves_steps.append(c);
    c=Curve();
    c.set_color(QColor(255,255,255));
    c.set_label("animation_pe");
    _curves_steps.append(c);

    c=Curve();
    c.set_color(QColor(255,0,0));
    c.set_label("Simulation KE");
    _curves_steps.append(c);
    c=Curve();
    c.set_color(QColor(0,0,255));
    c.set_label("Simulation ake");
    _curves_steps.append(c);
    c=Curve();
    c.set_color(QColor(255,255,255));
    c.set_label("Simulation pe");
    _curves_steps.append(c);

    c=Curve();
    c.set_color(QColor(255,0,255));
    c.set_label("ke diff");
    _curves_steps.append(c);
    c=Curve();
    c.set_color(QColor(0,255,255)); // cyan
    c.set_label("ake diff");
    _curves_steps.append(c);
    c=Curve();
    c.set_color(QColor(255,255,0)); // yellow
    c.set_label("pe diff");
    _curves_steps.append(c);

    _error_1._speed_error = 0;
    _error_1._ticks= 0;
    _error_2._speed_error = 0;
    _error_2._ticks= 0;

    _energy.ake_diff = 0;
    _energy.ke_diff = 0;
    _energy.length= 0;
    _energy.mean_error = 0;
    _energy.mean_error_2= 0;
    _energy.part_name= "";
    _energy.pe_diff = 0;
    _energy.animation.ake = 0;
    _energy.animation.ke = 0;
    _energy.animation.pe = 0;
    _energy.animation.speed = 0;
    _energy.animation.x = 0;
    _energy.animation.y = 0;
    _energy.animation.z = 0;
    _energy.simulation.ake = 0;
    _energy.simulation.ke = 0;
    _energy.simulation.pe = 0;
    _energy.simulation.speed = 0;
    _energy.simulation.x = 0;
    _energy.simulation.y = 0;
    _energy.simulation.z = 0;
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
    if(!_body || !_motion_state){
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
    ;
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
        shape.setX(((btCapsuleShape *)_shape)->getRadius());
        shape.setY(((btCapsuleShape *)_shape)->getHalfHeight()*2);
        shape.setZ(((btCapsuleShape *)_shape)->getRadius());
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
        _shape = new btCapsuleShape(shape.x(),shape.y());
        break;
    }

}

const InteractiveObject::t_part_info& InteractiveObject::updatePartInfo(float elapsed,float diff,float gravity){
    btVector3 distance(_animation.translationVector(elapsed)-_previous_data._position);
    btVector3 simulation_distance = _body->getCenterOfMassPosition()-_previous_data._position_simulation;
    btVector3 speed_animation(distance/(diff/1000)); // the diff value is in ms so a conversion is needed to be in m/s
//    qDebug()<<"diff"<<diff;
    _animation_speed = speed_animation;


/// Calculated simulation speed
    _calculated_simulation_speed = simulation_distance/(diff/1000);
    _error_1._speed_error += absolute_value(_calculated_simulation_speed.length() - _body->getLinearVelocity().length());
    _previous_data._position = _animation.translationVector(elapsed);
    _previous_data._position_simulation_2 = _previous_data._position_simulation;
    _previous_data._position_simulation = _body->getCenterOfMassPosition();

    btVector3 simulation_distance_2 = _body->getCenterOfMassPosition()-_previous_data._position_simulation_2;
    btVector3 _calculated_simulation_speed_2 = simulation_distance_2 / (2*diff/1000);
    _error_2._speed_error += absolute_value(_calculated_simulation_speed_2.length() - _previous_data._linear_velocity.length());
    _previous_data._linear_velocity = _body->getLinearVelocity();
///

// There are 3 ways of getting the (simulation) speed of the part:
// 1 - getting it from the getLinearVelocity() method of the body class
// 2 - calculating it with the formula s = ( current_position - previous position ) / distance
// 3 - formula s(-1) = ( current_position - previous previous position) / (distance*2)
//    btVector3 speed_simulation =_body->getLinearVelocity(); // first method
    btVector3 speed_simulation =_calculated_simulation_speed; // second method
//    btVector3 speed_simulation =_calculated_simulation_speed_2; // third method

    _angular_speed_rotation = (_animation.rotationVector(elapsed) - _previous_data._rotation_animation) / (diff/1000) ;
//    qDebug()<<"rotation :";
//    qDebug()<<_animation.rotationVector(elapsed).x();
//    qDebug()<<_animation.rotationVector(elapsed).y();
//    qDebug()<<_animation.rotationVector(elapsed).z();
//    qDebug();
//    qDebug()<<_previous_data._rotation_animation.x();
//    qDebug()<<_previous_data._rotation_animation.y();
//    qDebug()<<_previous_data._rotation_animation.z();
//    qDebug();
//    qDebug()<<_angular_speed_rotation.x();
//              qDebug()<<_angular_speed_rotation.y();
//              qDebug()<<_angular_speed_rotation.z();

    _previous_data._rotation_animation = _animation.rotationVector(elapsed);
    _energy.animation.speed = speed_animation.length();
    _energy.simulation.speed = speed_simulation.length();

    _energy.animation.ke  = kinetic_energy( speed_animation.length(),                    _mass);
    _energy.simulation.ke = kinetic_energy( speed_simulation.length(),_mass);

    _energy.animation.pe  = potential_energy(_mass,-gravity,_animation.translationVector(elapsed).y());
    _energy.simulation.pe = potential_energy(_mass,-gravity,_body->getCenterOfMassPosition().y()     );

    _energy.animation.ake = get_angular_EC_animation();
    _energy.simulation.ake = get_angular_EC_simulation();

    _energy.animation.x = _animation.translationVector(elapsed).x();
    _energy.animation.y = _animation.translationVector(elapsed).y();
    _energy.animation.z = _animation.translationVector(elapsed).z();

    _energy.simulation.x = _body->getCenterOfMassPosition().x();
    _energy.simulation.y = _body->getCenterOfMassPosition().y();
    _energy.simulation.z = _body->getCenterOfMassPosition().z();

    _energy.part_name = _body_part_name;
    _animation_from_simulation.get_translation_curves()[0].insert(elapsed,_body->getCenterOfMassPosition().x());
    _animation_from_simulation.get_translation_curves()[1].insert(elapsed,_body->getCenterOfMassPosition().y());
    _animation_from_simulation.get_translation_curves()[2].insert(elapsed,_body->getCenterOfMassPosition().z());

    _energy.ake_diff = _energy.animation.ake - _energy.simulation.ake;
    _energy.ke_diff = _energy.animation.ke - _energy.simulation.ke;
    _energy.pe_diff = _energy.animation.pe - _energy.simulation.pe;

    ++_error_1._ticks;
    ++_error_2._ticks;
    _energy.mean_error = get_mean_error();
    _energy.mean_error_2 = get_mean_error_2();

//    if (GlobalConfig::is_enabled("display_speed"))
//        _curves[0].insert(elapsed,_energy.simulation.speed);
//    if (GlobalConfig::is_enabled("display_error"))
//        _curves[3].insert(elapsed,_energy.mean_error);
    if (GlobalConfig::is_enabled("display_animation_stats")) {
        _curves[0].insert(elapsed,_energy.animation.ke);
        _curves[1].insert(elapsed,_energy.animation.ake);
        _curves[2].insert(elapsed,_energy.animation.pe);
    }
    if (GlobalConfig::is_enabled("display_simulation_stats")) {
        _curves[3].insert(elapsed,_energy.simulation.ke);
        _curves[4].insert(elapsed,_energy.simulation.ake);
        _curves[5].insert(elapsed,_energy.simulation.pe);
    }
    if (GlobalConfig::is_enabled("display_diff")) {
        _curves[6].insert(elapsed,_energy.ke_diff);
        _curves[7].insert(elapsed,_energy.ake_diff);
        _curves[8].insert(elapsed,_energy.pe_diff);
    }
    return _energy;
}

//btScalar InteractiveObject::computeWork(btScalar ke_simulation , btScalar ke_animation , btScalar ake_simulation , btScalar ake_animation , btScalar pe_simulation , btScalar pe_animation){
//    return (ke_simulation - ke_animation)+ (ake_simulation - ake_animation ) + (pe_simulation - pe_animation) ;
//}


void InteractiveObject::setSimulationPosition(float time){

    if (GlobalConfig::is_enabled("display_animation_stats")) {
        _curves_steps[0].insert(time,_energy.animation.ke);
        _curves_steps[1].insert(time,_energy.animation.ake);
        _curves_steps[2].insert(time,_energy.animation.pe);
    }
    if (GlobalConfig::is_enabled("display_simulation_stats")) {
        _curves_steps[3].insert(time,_energy.simulation.ke);
        _curves_steps[4].insert(time,_energy.simulation.ake);
        _curves_steps[5].insert(time,_energy.simulation.pe);
    }
    if (GlobalConfig::is_enabled("display_diff")) {
        _curves_steps[6].insert(time,_energy.ke_diff);
        _curves_steps[7].insert(time,_energy.ake_diff);
        _curves_steps[8].insert(time,_energy.pe_diff);
    }


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
//        body.setLinearVelocity(_animation_speed);
//        body.setAngularVelocity(btVector3(deg2rad(_angular_speed_rotation.y()),
//                                          deg2rad(_angular_speed_rotation.x()),
//                                          deg2rad(_angular_speed_rotation.z())));
//        qDebug()<<" rotation "<<_angular_speed_rotation.length();
        _previous_data._linear_velocity = _animation_speed; // sets the previous speed to the same as currens speed to avoid calculation errors
    } else {
        btRigidBody& body = get_body();
        initialSpeed();
        body.setLinearVelocity(_animation_speed);
//        body.setAngularVelocity(btVector3(deg2rad(_angular_speed_rotation.y()),
//                                          deg2rad(_angular_speed_rotation.x()),
//                                          deg2rad(_angular_speed_rotation.z())));
//        qDebug()<<" rotation "<<_angular_speed_rotation.length();
        _previous_data._linear_velocity = _animation_speed; // sets the previous speed to the same as currens speed to avoid calculation errors
    }

    _previous_data._position = translation;
    _previous_data._position_simulation = translation;
    _previous_data._position_simulation_2 = translation;
//    get_body().setLinearVelocity(btVector3(0,9,0));

}

btVector3 InteractiveObject::initialSpeed(){
    _animation.get_translation_curves();
    btVector3 init_position = _animation.translationVector(0);
    btScalar diff = _animation.get_translation_curves()[0].keys()[1];
    btVector3 second = _animation.translationVector(diff);
    btVector3 distance(second-init_position);
    btVector3 speed_animation(distance/(diff/1000)); // the diff value is in ms so a conversion is needed to be in m/s
    _animation_speed = speed_animation;
    return _animation_speed;
}
