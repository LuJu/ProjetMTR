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
        float x,y,z;
        float speed;
        float ake,ke,pe;
    }t_energy_info;

    typedef struct part_info{
        QString part_name;
        t_energy_info animation, simulation;
        float work;
        float mean_error;
        float mean_error_2;
    }t_part_info;

    QList<Curve> _curves;

    enum shapetype{
        cube,
        cylinder,
        capsule
    };

    InteractiveObject(const btVector3 &origin, const btVector3 &shape=btVector3(1,1,1),shapetype type=cube);
    InteractiveObject();
    ~InteractiveObject();
    void __build(const btVector3& origin, const btVector3& shape,shapetype type);

    //! brief
    /*!
        description
        \n Status  1 : not implemented
        \param  input
        \return output
    */
    void buildMotion();    

    //! calculates the information about the energy of the part of the body
    /*!
        calculates the information about the energy of the part of the body \n
        This is where the kinetic energy and speed and potential energy are calculated
        \n Status  1 : not implemented
        \param  input
        \return output
    */
    const InteractiveObject::t_part_info& updatePartInfo(float elapsed,float diff,float gravity);

    btVector3 _previous_position;


    btVector3 get_previous_position_simulation() const {return _previous_position_simulation;}
    void set_previous_position_simulation(btVector3 previous_position_simulation){_previous_position_simulation = previous_position_simulation;}

    btVector3 _calculated_simulation_speed;
    //! error between the speed given by bullet and the speed obtained from calculation
    btScalar _speed_error;
    btScalar _speed_error_2;
    btScalar _ticks;
    btScalar _ticks_2;

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

    InteractiveObject::t_part_info getEnergyInformation() const {return _energy;}

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
    btScalar get_mean_error_2() const {
        return _speed_error_2 / _ticks_2 ;
    }

//    btVector3 get_animation_speed() const {return _animation_speed;}
//    void set_animation_speed(btVector3 animation_speed){_animation_speed = animation_speed;}



private:
    void deleteMotion();

    //! temporary disabled
    InteractiveObject(const InteractiveObject& object);

    btVector3 _animation_speed;
    btVector3 _previous_linear_velocity;
    btVector3 _previous_position_simulation;
    btVector3 _previous_position_simulation_2;

    btTransform _transform;
    // _shape needs to have a polymorphical behaviour so it is a pointer
    btCollisionShape * _shape;
    btVector3 _local_inertia;
    btScalar _mass;
    btDefaultMotionState * _motion_state;
    btRigidBody * _body;

    bool _animated;
    t_part_info _energy;

    QString _body_part;


};

#endif // INTERACTIVEOBJECT_H
