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

#include "animatedobject.h"
#include "simulatedobject.h"
#include "shape.h"
#include "utils.h"

class InteractiveObject
{
public:
    InteractiveObject(const btVector3 &origin, const btVector3 &shape=btVector3(1,1,1),Shape::shapetype type=Shape::cube);
    InteractiveObject();
    ~InteractiveObject();

    //! calculates the information about the energy of the part of the body
    /*!
        calculates the information about the energy of the part of the body \n
        This is where the kinetic energy and speed and potential energy are calculated
        \n Status  1 : not implemented
        \param  input
        \return output
    */
    void updatePartInfo(float elapsed,float diff,float gravity);
    t_part_info getEnergyInformation() const {return _energy;}

    btVector3 & get_local_inertia() {return _local_inertia;}
    void set_local_inertia(btVector3 local_inertia){_local_inertia = local_inertia;}

    btScalar get_mass() const {return _mass;}
    void set_mass(btScalar mass){_mass = mass;}

    btTransform & get_transform() {return _transform;}
    void set_transform(btTransform transform){_transform = transform;}

    btRigidBody & get_body();
    const btRigidBody & get_body() const ;

    AnimatedObject& get_animation() {return _animation;}

    bool get_animated() const {return _animated;}
    void set_animated(bool animated){_animated = animated;}

    const QString& get_body_part() const {return _body_part_name;}
    void set_body_part(const QString& body_part){_body_part_name = body_part;}

    btDefaultMotionState * get_motion_state(){
        if(!_motion_state)
            qWarning()<<"Motion state not defined";
        return _motion_state;
    }

    const QList<Curve>& get_curves() const {return _curves;}
    const QList<Curve>& get_curves_steps() const {return _curves_steps;}

    QString exportSimulationToAnimation();


    MeshPointer& get_mesh(){return _mesh;}
    Shape& get_shape_struct(){return _shape;}

    void buildMesh();
    void setSimulationPosition(float time);

private:

    enum curve_data{
        ANIMATION_KE = 0,
        ANIMATION_AKE,
        ANIMATION_PE,
        SIMULATION_KE,
        SIMULATION_AKE,
        SIMULATION_PE,
        DIFF_KE,
        DIFF_AKE,
        DIFF_PE,
        ANIMATION_X,
        ANIMATION_Y,
        SIMULATION_Y,
        NUMBER_OF_CURVES
    };

    void __build(const btVector3& origin, const btVector3& shape,Shape::shapetype type);
    Shape _shape;
    void deleteMotion();
    void buildMotion();
    MeshPointer _mesh;

    btVector3 speedAtTime(float time) const;
    //! disabled
    InteractiveObject(const InteractiveObject& object);
    btDefaultMotionState * _motion_state;
    btTransform _transform;

    btVector3 _local_inertia;
    btRigidBody * _body;
    btCollisionShape *_collision_shape;
    // Curves used to display data on screen
    QList<Curve> _curves;
    QList<Curve> _curves_steps;
    AnimatedObject _animation;
    SimulatedObject _simulation;
    AnimatedObject _animation_from_simulation;
    QString _body_part_name;
    btScalar _mass;
    bool _animated;

    t_part_info _energy;


    btVector3 _calculated_simulation_speed;


    void appendCurve(QList<Curve>& list, int index, QString label, QColor color);
    void appendCurves(QList<Curve>& list);
    void insertDataToCurves(QList<Curve>& curves, float elapsed);
    void updateEnergyStructure(float gravity);
    void updateAnimationFromSimulationData(float time);
    void setSimulationTransformFromAnimation(float time);
    btScalar get_density(){
        return _mass / _shape.get_volume();
    }

    btScalar get_moment(btVector3 rotation_axis);

    btScalar get_angular_EC_simulation();

    btScalar get_angular_EC_animation();

    btVector3 get_extremity_animation(float time) const ;
    void updateSimulation(float elapsed,float delta_t,float gravity);
    void updateAnimation(float elapsed,float delta_t,float gravity);
};

#endif // INTERACTIVEOBJECT_H
