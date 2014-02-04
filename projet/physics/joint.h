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
    
};

#endif // JOINT_H
