#ifndef JOINT_H
#define JOINT_H

#include <QObject>
#include <QPair>

#include "interactiveobject.h"
#include "animatedobject.h"
#include "simulatedobject.h"
#include "debuggingutils.h"


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
    bool buildConstraint();
    void allocateConstraint();

    QPair<InteractiveObject* ,InteractiveObject* > _parts;
    constraints_types _type;
    Joint& operator=( const Joint& other ) ;

    bool has_parts() const ;
    bool has_parent() const {
        if (_parent_part_name == "root" || _parent_part_name=="") return false;
        return true;
    }



    AnimatedObject& get_animation() {return _animation;}
    AnimatedObject _animation;
    SimulatedObject _simulation;
    QString _part_name;
    QString _parent_part_name;



private:
    btTypedConstraint * _constraint;
    void deleteConstraint();
};

#endif // JOINT_H
