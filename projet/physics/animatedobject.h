#ifndef ANIMATEDOBJECT_H
#define ANIMATEDOBJECT_H

#include "btBulletCollisionCommon.h"

#include "utils/triplet.h"
#include "utils/curve.h"

#include "utils.h"



class AnimatedObject
{
public:
    AnimatedObject();

    btVector3 scalingVector(float time) const {
        return btVector3(_scaling_curves[0].get_value(time),
                         _scaling_curves[1].get_value(time),
                         _scaling_curves[2].get_value(time));
    }
    btVector3 translationVector(float time) const {
        return btVector3(_translation_curves[0].get_value(time),
                         _translation_curves[1].get_value(time),
                         _translation_curves[2].get_value(time));
    }
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
        return btVector3(_translation_curves[0].tangentAt(time),
                         _translation_curves[1].tangentAt(time),
                         _translation_curves[2].tangentAt(time));
    }
    btVector3 rotationTangent(float time) const {
        return btVector3(_rotation_curves[0].tangentAt(time),
                         _rotation_curves[1].tangentAt(time),
                         _rotation_curves[2].tangentAt(time));
    }
    Curve3d& get_scaling_curves() {return _scaling_curves;}
    Curve3d& get_rotation_curves() {return _rotation_curves;}
    Curve3d& get_translation_curves() {return _translation_curves;}

    state_data _current_state, _previous_state;


private:
    Curve3d _scaling_curves;
    Curve3d _rotation_curves;
    Curve3d _translation_curves;

};

#endif // ANIMATEDOBJECT_H
