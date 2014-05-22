#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <QPair>
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "interactiveobject.h"
#include "joint.h"

class Constraint
{
public:
    Constraint();
    Constraint(InteractiveObject * first, InteractiveObject * second, bool first_base = false, bool second_base = false);
    Constraint& operator=( const Constraint& other ) ;
    enum constraints_types{
        hinge,
        point,
        cone
    };

    explicit Constraint(const Constraint& other);
    virtual ~Constraint();

    btTypedConstraint* get_constraint() const{
        if (!_constraint){
            qWarning()<<"requesting uninitialized constraint pointer";
        }
        return _constraint;
    }
    bool buildConstraint();
    void allocateConstraint();

    QPair<InteractiveObject* ,InteractiveObject* > _parts;
    constraints_types _type;
    bool has_parts() const;
    bool _first_base,_second_base;
private:
    btTypedConstraint * _constraint;
    void deleteConstraint();
};

#endif // CONSTRAINT_H


