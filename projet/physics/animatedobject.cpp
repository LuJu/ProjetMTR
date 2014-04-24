#include "animatedobject.h"

AnimatedObject::AnimatedObject() {
//    _scaling_curves.set_interpolation(Curve::bezier);
    _extremity_translation_curves.set_interpolation(Curve::bezier);
    _rotation_curves.set_interpolation(Curve::bezier);
}

const btTransform AnimatedObject::getWorldBaseTransform(const btTransform parent, float time) const{
    btTransform final_transform, object_transform;
    btQuaternion quat;
    btVector3 rotation;
    btVector3 buffer;

    object_transform.setIdentity();
    object_transform.setOrigin(extremityTranslationVector(time));

    rotation = rotationVector(time);
    quat.setEuler(deg2rad(rotation.y()),deg2rad(rotation.x()),deg2rad(rotation.z()));
    quat.normalize();
    object_transform.setRotation(quat);
    buffer = btQuat2euler(quat);
//    buffer = btVector3(rad2deg(buffer.x()),rad2deg(buffer.y()),rad2deg(buffer.z()));
    toString(rotation,"before");
    toString(buffer,"after");

    final_transform.setIdentity();
//    final_transform.setOrigin(parent.getOrigin() + object_transform.getOrigin());
//    final_transform.setRotation(parent.getRotation() * object_transform.getRotation());
//    final_transform = object_transform  * parent;
    btTransform parent_inv = parent.inverse();
    final_transform = parent * object_transform  ;

    return final_transform;
}
