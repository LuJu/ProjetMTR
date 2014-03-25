#ifndef SHAPE_H
#define SHAPE_H

#include "btBulletDynamicsCommon.h"


class Shape
{
public:
    enum shapetype{
        cube,
        cylinder,
        capsule
    };


    Shape():
//        _shape_collision(NULL),
        _shape(cube) {
        set_shape(btVector3(0,0,0));
    }
    Shape(btVector3 shape, shapetype type):
//        _shape_collision(NULL),
//        collision_shape_outdated(true),
        _shape(type){
        set_shape(shape);
    }

    ~Shape(){
//        if (_shape_collision)
//            delete _shape_collision;
    }

    shapetype get_shape_type() const {
        return _shape;
    }
    void set_shape_type(shapetype type){

        _shape = type;
//        collision_shape_outdated = true;
    }

    float get_density(float mass) const;
    float get_volume() const;
//    float get_radius() const;
//    float get_length() const;
    btCollisionShape * newCollisionShape();
    btCollisionShape * buildCollisionShape();
    shapetype _shape;
    btVector3 get_shape() const;
    void set_shape(const btVector3 &shape);
    btScalar _radius;
    btScalar _length;
    btVector3 _shape_vector; // cube

//    bool collision_shape_outdated;

private:
//    btCollisionShape * _shape_collision;

};

#endif // HAPE_H
