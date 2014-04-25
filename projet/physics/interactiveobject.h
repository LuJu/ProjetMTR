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

/*! \class InteractiveObject
        This class contains the definition of an object in the scene, (both the simulation and the animation data \n
        The animation data is containted in the _animation object, and the simulation object is defined by several objects _motion_state, _body etc)
    */
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
    */
    void updatePartInfo(float elapsed, float diff, float gravity, const btTransform &transform);
    t_part_info getEnergyInformation() const {return _energy;}

    btVector3 & get_local_inertia() {return _local_inertia;}
    void set_local_inertia(btVector3 local_inertia){_local_inertia = local_inertia;}

    btScalar get_mass() const {return _mass;}
    void set_mass(btScalar mass){_mass = mass;}

    btTransform & get_transform() {return _transform;}
    void set_transform(btTransform transform){_transform = transform;}

    btRigidBody & get_body();
//    const btRigidBody & get_body() const ;

    AnimatedObject& get_animation() {return _animation;}

    bool get_animated() const {return _animated;}
    void set_animated(bool animated){_animated = animated;}

    const QString& get_body_part() const {return _body_part_name;}
    void set_body_part(const QString& body_part){_body_part_name = body_part;}
    const QString& get_parent_body_part() const {return _parent_body_part_name;}
    void set_parent_body_part(const QString& parent_body_part_name){_parent_body_part_name = parent_body_part_name;}

    btDefaultMotionState * get_motion_state(){
        if(!_motion_state)
            qWarning()<<"Motion state not defined";
        return _motion_state;
    }

    const QList<Curve>& get_curves() const {return _curves;}
    const QList<Curve>& get_curves_steps() const {return _curves_steps;}

    QString exportSimulationToAnimation();


    //! Returns a smart pointer to the mesh representing the object
    MeshPointer& get_mesh(){return _mesh;}
    Shape& get_shape_struct(){return _shape;}

    void buildMesh();
    void setSimulationPosition(float time);
    AnimatedObject _animation;
    SimulatedObject _simulation;
    void setSimulationPosition2(btTransform transform, float time);
    void setInitialPosition(btTransform transform);

    int _center_of_mass_proportion;

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
    //! disabled
    InteractiveObject(const InteractiveObject& object);
    void __build(const btVector3& origin, const btVector3& shape,Shape::shapetype type);
    void deleteMotion();
    void buildMotion();

    Shape _shape;
    MeshPointer _mesh;

    //    returns the animation speed vector at time (might not be working)
    btVector3 speedAtTime(float time) const;

    btTransform _transform;

    // Objects used for the simulation
    btDefaultMotionState * _motion_state;
    btVector3 _local_inertia;
    btRigidBody * _body;
    btCollisionShape *_collision_shape;
    ////
    QList<Curve> _curves; //! The curves recorded at each update of the status of objects
    QList<Curve> _curves_steps; //! The curves recorded at each step of the simulation

    QString _body_part_name; //! Name of the body part
    QString _parent_body_part_name; //! Name of the parent
    btScalar _mass;
    bool _animated;

    t_part_info _energy;


    btVector3 _calculated_simulation_speed;


    void appendCurve(QList<Curve>& list, int index, QString label, QColor color);
    void appendCurves(QList<Curve>& list);
    void insertDataToCurves(QList<Curve>& curves, float elapsed);

    //! updates the structure that will be used for exportation
    void updateEnergyStructure(float gravity);
    void setSimulationTransformFromAnimation();
    btScalar get_density(){
        return _mass / _shape.get_volume();
    }

    btScalar get_angular_EC_simulation();

    btScalar get_angular_EC_animation();

    btVector3 get_extremity_animation() const ;
    void updateSimulation(float elapsed,float delta_t,float gravity);
    void updateAnimation(float elapsed, float delta_t, float gravity, const btTransform &transform);


};

#endif // INTERACTIVEOBJECT_H
