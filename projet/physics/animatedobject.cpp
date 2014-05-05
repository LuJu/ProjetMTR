#include "animatedobject.h"

AnimatedObject::AnimatedObject() {
    _extremity_translation_curves.set_interpolation(Curve::bezier);
    _rotation_curves.set_interpolation(Curve::bezier);
}

const btTransform AnimatedObject::getWorldTransform(const btTransform parent_transform, float time) const{
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

    final_transform.setIdentity();

    final_transform = parent_transform * object_transform  ;

    return final_transform;
}


