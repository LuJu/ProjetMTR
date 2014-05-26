/*
Copyright (c) 2013, Lucas Juliéron
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/
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

void Shape::deleteCollisionShape(btCollisionShape * collision_shape){
    switch (_shape){
        case cube:
            delete((btBoxShape*)collision_shape);
            break;
        case cylinder:
            delete((btCylinderShape*)collision_shape);
            break;
        case capsule:
            delete((btCapsuleShape*)collision_shape);
            break;
        case plane:
//            collision_shape  = new btStaticPlaneShape(btVector3(0,1,0),1);
        break;
    }
}
