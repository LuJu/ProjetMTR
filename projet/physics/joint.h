#ifndef JOINT_H
#define JOINT_H

#include <QObject>
#include <QPair>

#include "interactiveobject.h"


//! Class representing a joint between two objects
class Joint
{
public:
    enum constraints_types{
        hinge,
        point,
        cone
    };

    explicit Joint();
    explicit Joint(const Joint& other);
    ~Joint();
    QPair<InteractiveObject* ,InteractiveObject* > _parts;

    btVector3 get_position() const {
        btVector3 pos1 = _parts.first->get_body().getCenterOfMassTransform().getOrigin();
        btVector3 axis = _parts.first->get_body().getCenterOfMassTransform().getRotation().getAxis()*_parts.first->get_shape_struct().get_shape().y() ;
        btVector3 position = pos1 + axis;
        return position;
    }

    btTypedConstraint* get_constraint(){
        if (!_constraint){
            qWarning()<<"requesting uninitialized constraint pointer";
        }
        return _constraint;
    }
    void buildConstraint();
    btVector3 get_world_position();
    btVector3 get_world_position_simulation() const;

    constraints_types _type;
    btVector3 _pivotA;
    btVector3 _pivotB;
    btTransform _localeA;
    btTransform _localeB;

    btScalar _limit[5];
    bool _limited;
    bool _complete;

    Joint& operator=( const Joint& other ) ;



private:
    btTypedConstraint * _constraint;
    void deleteConstraint();

//    btTypedConstraint * _constraint_pointer;


};

#endif // JOINT_H
