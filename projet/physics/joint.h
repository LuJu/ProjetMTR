#ifndef JOINT_H
#define JOINT_H

#include <QObject>
#include <QPair>

#include "interactiveobject.h"
#include "animatedobject.h"
#include "simulatedobject.h"
#include "debuggingutils.h"
#include "constraint.h"



//! Class representing a joint between two objects
class Joint
{
public:
    explicit Joint();
    explicit Joint(const Joint& other);
    virtual ~Joint();

    Joint& operator=( const Joint& other ) ;
    bool has_parent() const {
        if (_parent_part_name == "root" || _parent_part_name=="") return false;
        return true;
    }

    AnimatedObject& get_animation() {return _animation;}
    AnimatedObject _animation;
    QString _part_name;
    QString _parent_part_name;
    InteractiveObject * _main_part;
    float _part_mass;
    float _part_com_proportion;


private:
};

#endif // JOINT_H
