#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H

#include <QDebug>

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "utils/geometry.h"
#include "utils/typedefinitions.h"
#include "utils/maths.h"
#include "utils/curve.h"

#include "3d/meshutils.h"

//#include "core/globalconfig.h"

#include "animationdata.h"

class InteractiveObject
{
public:
    typedef struct energy_info{
        float x,y,z;
        float speed;
        float aspeed; //angular speed
        float ake,ke,pe;
    }t_energy_info;

    typedef struct part_info{
        QString part_name;
        float length;
        t_energy_info animation, simulation;
        float ake_diff;
        float ke_diff;
        float pe_diff;
        float mean_error;
        float mean_error_2;
    }t_part_info;

    enum shapetype{
        cube,
        cylinder,
        capsule
    };


    void setSimulationPosition(float time);


    btVector3 getTopPosition(){
        btVector3 center = get_body().getCenterOfMassPosition();
//        btTransform transform = get_body().getan
        btQuaternion orientation = get_body().getOrientation();
        btVector3 position = orientation.getAxis() * get_shape().getY();
    }



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
    InteractiveObject::t_part_info getEnergyInformation() const {return _energy;}


//    btVector3 get_previous_position_simulation() const {return _previous_position_simulation;}
//    void set_previous_position_simulation(btVector3 previous_position_simulation){_previous_position_simulation = previous_position_simulation;}



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

    const QString& get_body_part() const {return _body_part_name;}
    void set_body_part(const QString& body_part){_body_part_name = body_part;}


    btDefaultMotionState * get_motion_state(){
        if(!_motion_state)
            _motion_state = new btDefaultMotionState(_transform);
        return _motion_state;
    }
    btScalar get_mean_error() const {
        return _error_1._speed_error / _error_1._ticks ;
    }
    btScalar get_mean_error_2() const {
        return _error_2._speed_error / _error_2._ticks ;
    }

//    btVector3 get_animation_speed() const {return _animation_speed;}
//    void set_animation_speed(btVector3 animation_speed){_animation_speed = animation_speed;}

    btScalar get_density(){
        return _mass / get_volume();
    }

    btScalar get_volume();
    btScalar get_moment();

    btScalar get_angular_EC_simulation();

    btScalar get_angular_EC_animation();


    QList<Curve>& get_curves() {return _curves;}
    QList<Curve>& get_curves_steps() {return _curves_steps;}


    shapetype get_shape_type() const {return _shape_type;}
    void set_shape_type(shapetype shape_type){_shape_type = shape_type;}
    btVector3 speedAtTime(float time);

    QString exportSimulationToAnimation();

    MeshPointer mesh;

    void buildMesh();

private:
    void deleteMotion();

    //! temporary disabled
    InteractiveObject(const InteractiveObject& object);
    btDefaultMotionState * _motion_state;
    // _shape needs to have a polymorphical behaviour so it is a pointer
    btCollisionShape * _shape;
    btTransform _transform;

    shapetype _shape_type;

    btVector3 _local_inertia;
    btRigidBody * _body;

    // Curves used to display data on screen
    QList<Curve> _curves;
    QList<Curve> _curves_steps;
    AnimationData _animation;
    AnimationData _animation_from_simulation;
    QString _body_part_name;
    btScalar _mass;
    bool _animated;

    t_part_info _energy;


    btVector3 _calculated_simulation_speed;
    btVector3 _animation_speed;
    btVector3 _angular_speed_animation;
    btScalar _angular_speed_y;


    struct{
        btVector3 _position;
        btVector3 _rotation_animation;
        btVector3 _linear_velocity;
        btVector3 _position_simulation;
        btVector3 _position_simulation_2;
    }_previous_data;

    //! error between the speed given by bullet and the speed obtained from calculation
    struct {
        btScalar _speed_error;
        btScalar _ticks;
    }_error_1,_error_2;

//    btVector3 _rotation_axis_animation;
//    btVector3 _rotation_axis_simulation;

    float rot_x(btQuaternion q) const;

    float rot_y(btQuaternion q) const;

    float rot_z(btQuaternion q) const;

    btVector3 btQuat2euler(btQuaternion q);
};

#endif // INTERACTIVEOBJECT_H
