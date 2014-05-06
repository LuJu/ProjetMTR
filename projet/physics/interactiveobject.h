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
#include "debuggingutils.h"

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
    btScalar get_mass() const {return _mass;}
    void set_mass(btScalar mass){_mass = mass;}
    btScalar get_center_of_mass_proportion() const {return _center_of_mass_proportion;}
    void set_center_of_mass_proportion(btScalar center_of_mass_proportion){_center_of_mass_proportion = center_of_mass_proportion;}

    const btTransform& get_original_transform() const {return _original_transform;}
    void set_original_transform(const btTransform& transform){_original_transform = transform;}

    btRigidBody * get_body(){ return _simulation.get_body(_mass,_original_transform,_center_of_mass_proportion); }

    AnimatedObject& get_animation() {return _animation;}

    bool get_animated() const {return _animated;}
    void set_animated(bool animated){_animated = animated;}

    const QString& get_body_part() const {return _body_part_name;}
    void set_body_part(const QString& body_part){_body_part_name = body_part;}
    const QString& get_parent_body_part() const {return _parent_body_part_name;}
    void set_parent_body_part(const QString& parent_body_part_name){_parent_body_part_name = parent_body_part_name;}

    btDefaultMotionState * get_motion_state() const{return _simulation.get_motion_state(); }

    const QList<Curve>& get_curves() const {return _curves;}
    const QList<Curve>& get_curves_steps() const {return _curves_steps;}

    QString exportSimulationToAnimation();


    //! Returns a smart pointer to the mesh representing the object
    MeshPointer& get_mesh(){return _mesh;}
    Shape& get_shape_struct(){return _shape;}

    void buildMesh();
    AnimatedObject _animation;
    SimulatedObject _simulation;
    void setSimulationPosition(btTransform transform, float time);
    void setInitialPosition(btTransform transform);

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

    btTransform _original_transform;

    QList<Curve> _curves; //! The curves recorded at each update of the status of objects
    QList<Curve> _curves_steps; //! The curves recorded at each step of the simulation

    QString _body_part_name; //! Name of the body part
    QString _parent_body_part_name; //! Name of the parent
    btScalar _mass;
    btScalar _center_of_mass_proportion;
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
    void updateSimulation(float delta_t);
    void updateAnimation(float delta_t, const btTransform &transform);


};

#endif // INTERACTIVEOBJECT_H
