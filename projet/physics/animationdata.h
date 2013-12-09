#ifndef ANIMATIONDATA_H
#define ANIMATIONDATA_H
#include "btBulletCollisionCommon.h"
#include "utils/triplet.h"
#include "utils/curve.h"

class Curve3d : public Triplet<Curve>
{
};

class AnimationData
{
public:
    AnimationData();

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
    Curve3d& get_scaling_curves() {return _scaling_curves;}
    Curve3d& get_rotation_curves() {return _rotation_curves;}
    Curve3d& get_translation_curves() {return _translation_curves;}

private:
    Curve3d _scaling_curves;
    Curve3d _rotation_curves;
    Curve3d _translation_curves;

};

#endif // ANIMATIONDATA_H
