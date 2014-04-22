#include "shape.h"


btScalar Shape::get_volume() const {
    switch (_shape){
    case capsule:
        return capsuleVolume();
        break;
    case cylinder:
        return cylinderVolume();
        break;
    case cube:
        return cubeVolume();
        break;
    }
}


btScalar Shape::capsuleVolume()const{

    // for capsule : volume of cylinder of radius R and height H + volume of sphere of radius R
    // = pi.R^2.h + 4/3.pi.R^3
    btScalar r = get_shape().x();
    btScalar h = get_shape().y();
    btScalar sphere_volume = (4/3) * M_PI * pow(r,3);
    btScalar cylinder_volume =  M_PI * pow(r,2) * h;
    return cylinder_volume+sphere_volume;
}
btScalar Shape::cylinderVolume()const{
    // = pi.R^2.h
    btScalar r = get_shape().x();
    btScalar h = get_shape().y();
    btScalar cylinder_volume =  M_PI * pow(r,2) * h;
    return cylinder_volume;
}
btScalar Shape::cubeVolume()const{
    btScalar x = get_shape().x();
    btScalar y = get_shape().y();
    btScalar z = get_shape().z();
    return x*y*z;
}


btVector3 Shape::get_shape() const {
    return _shape_vector;
}

void Shape::set_shape(const btVector3 &shape){
    _shape_vector = shape;
}

btCollisionShape *Shape::buildCollisionShape(){
    btCollisionShape * collision_shape ;
    switch (_shape){
        case cube:
            collision_shape = new btBoxShape(_shape_vector);
            break;
        case cylinder:
            collision_shape  = new btCylinderShape(_shape_vector);
            break;
        case capsule:
            collision_shape  = new btCapsuleShape(_shape_vector.x(),_shape_vector.y());
//            toString(_shape_vector,"shape");
            break;
        case plane:
            collision_shape  = new btStaticPlaneShape(btVector3(0,1,0),1);
        break;
    }
    return collision_shape;

}

btCollisionShape * Shape::newCollisionShape(){
    btCollisionShape * collision_shape = buildCollisionShape();
    return collision_shape;
}
