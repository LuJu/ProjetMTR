#include "animatedobject.h"

AnimatedObject::AnimatedObject() {
//    _scaling_curves.set_interpolation(Curve::bezier);
    _translation_curves.set_interpolation(Curve::bezier);
    _rotation_curves.set_interpolation(Curve::bezier);
}

const btTransform AnimatedObject::getWorldTransform(const btTransform parent, float time) const{
    btTransform transform_matrix, object_transform_matrix;

    btTransform transform;
    btQuaternion quat;
    btVector3 rotation;
    transform.setIdentity();
    transform.setOrigin(translationVector(time));
//    printbtVector3("vector",transform.getOrigin());
    rotation = rotationVector(time);
    quat.setEuler(deg2rad(rotation.y()),deg2rad(rotation.x()),deg2rad(rotation.z()));
    transform.setRotation(quat);
    object_transform_matrix = transform;
    transform_matrix = object_transform_matrix * parent;
//    printbtVector3("vector2",parent.getOrigin());
//    printbtVector3("final",transform_matrix.getOrigin());
    return transform_matrix;
}
