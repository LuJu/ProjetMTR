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
    btVector3 rotationVector(float time) const {
        return btVector3(_rotation_curves[0].get_value(time),
                         _rotation_curves[1].get_value(time),
                         _rotation_curves[2].get_value(time));
    }
    Curve3d& get_scaling_curves() {return _scaling_curves;}
    Curve3d& get_rotation_curves() {return _rotation_curves;}
    Curve3d& get_translation_curves() {return _extremity_translation_curves;}

    const btTransform getWorldTransform(const btTransform parent_transform, float time) const;

    void set_shape(Shape * shape){_shape = shape;}

    state_information _information;
private:
    Shape * _shape;
    Curve3d _scaling_curves;
    Curve3d _rotation_curves;
    Curve3d _extremity_translation_curves;

};

#endif // ANIMATEDOBJECT_H
