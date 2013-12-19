#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H

#include <QDebug>

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "utils/geometry.h"
#include "utils/typedefinitions.h"
#include "utils/maths.h"
#include "utils/curve.h"

#include "animationdata.h"

class InteractiveObject
{
public:

    void updateAnimationFromSimulation(float elapsed);
    void setSimulationPosition(float time);
    typedef struct energy_info{
        QString part_name;
        float animation_x;
        float animation_y;
        float animation_z;
        float simulation_x;
        float simulation_y;
        float simulation_z;
        float animation_speed;
        float animation_ke;
        float animation_ake;
        float animation_pe;
        float simulation_speed;
        float simulation_ke;
        float simulation_ake;
        float simulation_pe;
        float work;
        float mean_error;
    }t_energy_info;

    QList<Curve> _curves;

    enum shapetype{
        cube,
        cylinder
    };

    InteractiveObject(const btVector3 &origin=btVector3(0,0,0), const btVector3 &shape=btVector3(1,1,1),shapetype type=cube);
    ~InteractiveObject();

    //! brief
    /*!
        description
        \n Status  1 : not implemented
        \param  input
        \return output
    */
    void buildMotion();    
    const InteractiveObject::t_energy_info& update_energy_info(float elapsed,float diff,float gravity);

    btVector3 _previous_position;
    btVector3 _previous_position_simulation;
    btVector3 _animation_speed;
    btVector3 _calculated_simulation_speed;
    //! error between the speed given by bullet and the speed obtained from calculation
    btScalar _speed_error;
    btScalar _ticks;

    btVector3 get_shape() const ;
    void set_shape(const btVector3 &shape);

    btVector3 & get_local_inertia() {return _local_inertia;}
    void set_local_inertia(btVector3 local_inertia){_local_inertia = local_inertia;}

    btScalar get_mass() const {return _mass;}
    void set_mass(btScalar mass){_mass = mass;}

    btTransform & get_transform() {return _transform;}
    void set_transform(btTransform transform){_transform = transform;}

    btRigidBody & get_body();

    AnimationData& get_animation() {return _animation;}

    void set_animation(AnimationData& animation){_animation = animation;}

    bool get_animated() const {return _animated;}
    void set_animated(bool animated){_animated = animated;}

    const QString& get_body_part() const {return _body_part;}
    void set_body_part(const QString& body_part){_body_part = body_part;}

    InteractiveObject::t_energy_info getEnergyInformation() const {return _energy;}

    btDefaultMotionState * get_motion_state(){
        if(!_motion_state)
            _motion_state = new btDefaultMotionState(_transform);
        return _motion_state;
    }
    shapetype _shape_type;

    AnimationData _animation;
    AnimationData _animation_from_simulation;

    btScalar get_mean_error() const {
        return _speed_error / _ticks ;
    }


private:
    void deleteMotion();

    //! temporaly disabled
    InteractiveObject(const InteractiveObject& object);


    btTransform _transform;
    // _shape needs to have a polymorphical behaviour so it is a pointer
    btCollisionShape * _shape;
    btVector3 _local_inertia;
    btScalar _mass;
    btDefaultMotionState * _motion_state;
    btRigidBody * _body;

    bool _animated;
    t_energy_info _energy;

    QString _body_part;

};

#endif // INTERACTIVEOBJECT_H
