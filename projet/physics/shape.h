#ifndef SHAPE_H
#define SHAPE_H

#include "btBulletDynamicsCommon.h"


/*!
        Class representing a 3d shape (cube, capsule, etc)
    */
class Shape
{
public:

    //! defines the shape type
    enum shapetype{
        cube,
        cylinder,
        capsule,
        plane
    };

    Shape():
        _shape(cube) {
        set_shape(btVector3(0,0,0));
    }
    Shape(btVector3 shape, shapetype type):
        _shape(type){
        set_shape(shape);
    }

    ~Shape(){
    }

    shapetype get_shape_type() const {
        return _shape;
    }
    void set_shape_type(shapetype type){
        _shape = type;
    }

    float get_density(float mass) const;
    //! Returns the volume of the object
    float get_volume() const;
    //! Returns a pointer to a new collision shape instance following the definition of the Shape object, the caller takes the ownership of the instance returned
    btCollisionShape * newCollisionShape();

    /*! contains data of the size of the object \n
        The nature of the data depends on the type of the shape \n
        For a capsule : x containst the radius, y the length
    */
    btVector3 _shape_vector;

    btVector3 get_shape() const;
    void set_shape(const btVector3 &shape);
private:
//    btCollisionShape * _shape_collision;
    btCollisionShape * buildCollisionShape();
    btScalar cylinderVolume()const;
    btScalar cubeVolume() const;
    btScalar capsuleVolume()const;

    shapetype _shape;

    //! Not used
    btScalar _radius;
    //! Not used
    btScalar _length;
};

#endif // HAPE_H
