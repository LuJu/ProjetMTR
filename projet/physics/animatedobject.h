#ifndef ANIMATEDOBJECT_H
#define ANIMATEDOBJECT_H

#include "btBulletCollisionCommon.h"

#include "utils/triplet.h"
#include "utils/curve.h"

#include "utils.h"
#include "shape.h"


//! An animated 3D object
/*!
        Represents an animated 3D object. The values of the movement at each time is contained in the three 3DCurves
    */
class AnimatedObject
{
public:
    AnimatedObject();

    btVector3 scalingVector(float time) const {
        return btVector3(_scaling_curves[0].get_value(time),
                         _scaling_curves[1].get_value(time),
                         _scaling_curves[2].get_value(time));
    }
    btVector3 extremityTranslationVector(float time) const {
        return btVector3(_extremity_translation_curves[0].get_value(time),
                         _extremity_translation_curves[1].get_value(time),
                         _extremity_translation_curves[2].get_value(time));
    }
//    btVector3 centerOfMassTranslationVector(float time) const {
//        return btVector3(_translation_curves[0].get_value(time),
//                _translation_curves[1].get_value(time),
//                _translation_curves[2].get_value(time));
//    }
    btVector3 rotationVector(float time) const {
        return btVector3(_rotation_curves[0].get_value(time),
                         _rotation_curves[1].get_value(time),
                         _rotation_curves[2].get_value(time));
    }
    btVector3 scalingTangent(float time) const {
        return btVector3(_scaling_curves[0].tangentAt(time),
                         _scaling_curves[1].tangentAt(time),
                         _scaling_curves[2].tangentAt(time));
    }
    btVector3 translationTangent(float time) const {
        return btVector3(_extremity_translation_curves[0].tangentAt(time),
                         _extremity_translation_curves[1].tangentAt(time),
                         _extremity_translation_curves[2].tangentAt(time));
    }
    btVector3 rotationTangent(float time) const {
        return btVector3(_rotation_curves[0].tangentAt(time),
                         _rotation_curves[1].tangentAt(time),
                         _rotation_curves[2].tangentAt(time));
    }
    Curve3d& get_scaling_curves() {return _scaling_curves;}
    Curve3d& get_rotation_curves() {return _rotation_curves;}
    Curve3d& get_translation_curves() {return _extremity_translation_curves;}

    state_data _current_state, _previous_state;
    const btTransform getWorldBaseTransform(const btTransform parent,float time) const;

    Shape * _shape;

    btVector3 comSpeedAtTime(float time) const{
        btVector3 v1 = centerToBaseVector(time);
        btVector3 v2 = centerToBaseVector(time + 100);
        btVector3 d = v1-v2;
        return d/100;
    }

//    btVector3 comPositionAtTime(float time){

//    }

    btVector3 centerToBaseVector(float time)const{

        btVector3 ypr = rotationVector(time);
        btQuaternion quat;

        quat.setEuler(deg2rad(ypr.y()),deg2rad(ypr.x()),deg2rad(ypr.z()));
        btScalar R = _shape->get_shape().y()/2;
        QVector3D base_position(0,-R,0);
        quat.normalize();
        QQuaternion qquat(quat.w(),quat.getAxis().x(),quat.getAxis().y(),quat.getAxis().z());
        qquat= qquat.fromAxisAndAngle(quat.getAxis().x(),quat.getAxis().y(),quat.getAxis().z(),rad2deg(quat.getAngle()));

        return btVector3(qquat.rotatedVector(base_position).x(),qquat.rotatedVector(base_position).y(),qquat.rotatedVector(base_position).z());
    }

    btVector3 centerToBaseVector()const{

        btVector3 ypr = _current_state._rotation;
        btQuaternion quat;

        quat.setEuler(deg2rad(ypr.y()),deg2rad(ypr.x()),deg2rad(ypr.z()));
        btScalar R = _shape->get_shape().y()/2;
        QVector3D base_position(0,-R,0);
        quat.normalize();
        QQuaternion qquat(quat.w(),quat.getAxis().x(),quat.getAxis().y(),quat.getAxis().z());
        qquat= qquat.fromAxisAndAngle(quat.getAxis().x(),quat.getAxis().y(),quat.getAxis().z(),rad2deg(quat.getAngle()));

        return btVector3(qquat.rotatedVector(base_position).x(),qquat.rotatedVector(base_position).y(),qquat.rotatedVector(base_position).z());
    }


private:
    Curve3d _scaling_curves;
    Curve3d _rotation_curves;
    Curve3d _extremity_translation_curves;
    Curve3d _center_of_mass_translation_curves;

};

#endif // ANIMATEDOBJECT_H
