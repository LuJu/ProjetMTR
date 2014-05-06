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


    //! Return the vector representing the current distance between the center of the shape and the base of the shape, in world coordinates
    /*!
        \return : The vector center->base
    */
    btVector3 centerToBaseVector()const{

        btVector3 ypr = _information._current._rotation;
        btQuaternion quat;

        quat.setEuler(ypr.y(),ypr.x(),ypr.z());
        btScalar R = _shape->get_shape().y()/2;
        QVector3D base_position(0,-R,0);
        quat.normalize();
        QQuaternion qquat(quat.w(),quat.getAxis().x(),quat.getAxis().y(),quat.getAxis().z());
        qquat= qquat.fromAxisAndAngle(quat.getAxis().x(),quat.getAxis().y(),quat.getAxis().z(),rad2deg(quat.getAngle()));

        return btVector3(qquat.rotatedVector(base_position).x(),qquat.rotatedVector(base_position).y(),qquat.rotatedVector(base_position).z());
    }

    void set_shape(Shape * shape){_shape = shape;}

    state_information _information;
private:
    Shape * _shape;
    Curve3d _scaling_curves;
    Curve3d _rotation_curves;
    Curve3d _extremity_translation_curves;
    Curve3d _center_of_mass_translation_curves;

};

#endif // ANIMATEDOBJECT_H
