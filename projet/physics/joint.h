#ifndef JOINT_H
#define JOINT_H

#include <QObject>
#include <QPair>

#include "interactiveobject.h"

class Joint
{
public:
    explicit Joint();
    QPair<InteractiveObject* ,InteractiveObject* > _parts;
    btPoint2PointConstraint * _constraint;
    

    btVector3 get_position() const {
        btVector3 pos1 = _parts.first->get_body().getCenterOfMassTransform().getOrigin();
        btVector3 axis = _parts.first->get_body().getCenterOfMassTransform().getRotation().getAxis()*_parts.first->get_shape().y() ;
        btVector3 position = pos1 + axis;
        return position;
    }
};

#endif // JOINT_H
