#ifndef JOINT_H
#define JOINT_H

#include <QObject>
#include <QPair>

#include "interactiveobject.h"

class Joint
{
public:
    enum constraints_types{
        hinge,
        point,
        cone
    };

    explicit Joint();
    ~Joint();
    QPair<InteractiveObject* ,InteractiveObject* > _parts;
    constraints_types _type;


    btVector3 _pivotA;
    btVector3 _pivotB;
    btTransform _localeA;
    btTransform _localeB;

    btScalar _limit[5];
    bool _limited;
    bool _complete;


    btVector3 get_position() const {
        btVector3 pos1 = _parts.first->get_body().getCenterOfMassTransform().getOrigin();
        btVector3 axis = _parts.first->get_body().getCenterOfMassTransform().getRotation().getAxis()*_parts.first->get_shape_struct().get_shape().y() ;
        btVector3 position = pos1 + axis;
        return position;
    }

    btTypedConstraint* get_constraint(){
        if (!_constraint)
            //build constraint
        return _constraint;
    }

    void buildConstraint();
    btVector3 get_world_position();
    btVector3 get_world_position_simulation() const;
private:
    btTypedConstraint * _constraint;

//    btTypedConstraint * _constraint_pointer;


};

#endif // JOINT_H
