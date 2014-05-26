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
#include "animatedobject.h"

AnimatedObject::AnimatedObject() {
    _extremity_translation_curves.set_interpolation(Curve::bezier);
//    _rotation_curves.set_interpolation(Curve::bezier);
}

const btTransform AnimatedObject::getWorldTransform(const btTransform parent_transform, float time) const{
    btTransform final_transform, object_transform;
    btQuaternion quat;
    btVector3 rotation;

    object_transform.setIdentity();
    object_transform.setOrigin(extremityTranslationVector(time));

//    rotation = xyz2yxz(deg2rad(rotationVector(time)));
    rotation = xyz2yxz(deg2rad(rotationVector(time)));
//    rotation = deg2rad(rotationVector(time));

    btVector3 xyz = btVector3(deg2rad(rotationVector(time)));


    ///////////
    btQuaternion bx(btVector3(1,0,0),xyz.x()),
                 by(btVector3(0,1,0),xyz.y()),
                 bz(btVector3(0,0,1),xyz.z());

//    a = r.getBasis();
//    btQuaternion quat;
    quat = bz * by * bx;
//    btQuaternion test;
//    test.setEuler(xyz.y(),xyz.x(),xyz.z());
//    btTransform rfinal;
//    rfinal.setBasis(b);
//    btVector3 result = btQuat2euler(quat);
//    //    return result;
//    return btVector3(result.y(),result.x(),result.z());


    //////




//    quat.setEuler(rotation.y(),rotation.x(),rotation.z());
    quat.normalize();
    object_transform.setRotation(quat);

    final_transform.setIdentity();

    final_transform = parent_transform * object_transform  ;

    return final_transform;
}


