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
#ifndef SHAPE_H
#define SHAPE_H

#include "btBulletDynamicsCommon.h"
#include "utils.h"


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

    virtual ~Shape(){
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
    void deleteCollisionShape(btCollisionShape * collision_shape);
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

class CapsuleShape : public Shape{



};

#endif // HAPE_H
