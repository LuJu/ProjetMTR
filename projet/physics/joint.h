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
    virtual ~Joint();

    btTypedConstraint* get_constraint() const{
        if (!_constraint){
            qWarning()<<"requesting uninitialized constraint pointer";
        }
        return _constraint;
    }
    void buildConstraint();

    QPair<InteractiveObject* ,InteractiveObject* > _parts;
    constraints_types _type;
    Joint& operator=( const Joint& other ) ;
private:
    btTypedConstraint * _constraint;
    void deleteConstraint();
};

#endif // JOINT_H
