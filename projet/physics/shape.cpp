#include "shape.h"


btScalar Shape::get_volume() const {
    // for capsule : volume of cylinder of radius R and height H + volume of sphere of radius R
    // = pi.R^2.h + 4/3.pi.R^3
    btScalar r = get_shape().x();
    btScalar h = get_shape().y();
    btScalar sphere_volume = (4/3) * M_PI * pow(r,3);
    btScalar cylinder_volume =  M_PI * pow(r,2) * h;
    return cylinder_volume+sphere_volume;
}


btVector3 Shape::get_shape() const {
    return _shape_vector;
}

void Shape::set_shape(const btVector3 &shape){
    _shape_vector = shape;
    if (_shape_collision!=NULL) delete _shape_collision;
    collision_shape_outdated = true;
}

void Shape::buildCollisionShape(){
    if (collision_shape_outdated){
        if (_shape_collision) delete _shape_collision;
        switch (_shape){
        case cube:
            _shape_collision= new btBoxShape(_shape_vector);
            break;
        case cylinder:
            _shape_collision = new btCylinderShape(_shape_vector);
            break;
        case capsule:
            _shape_collision = new btCapsuleShape(_shape_vector.x(),_shape_vector.y());
            break;
        }
        collision_shape_outdated = false;
    }
}

btCollisionShape * Shape::get_collision_shape(){
    buildCollisionShape();
    return _shape_collision;
}
