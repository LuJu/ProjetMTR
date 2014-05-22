#include "animatedobject.h"

AnimatedObject::AnimatedObject() {
//    _extremity_translation_curves.set_interpolation(Curve::bezier);
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


