/*
Copyright (c) 2013, Lucas Juliéron
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef PART_H
#define PART_H

#include <QDebug>

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "utils/geometry.h"
#include "utils/typedefinitions.h"
#include "utils/maths.h"
#include "utils/curve.h"

#include "3d/meshutils.h"

#include "simulatedobject.h"
#include "shape.h"
#include "utils.h"
#include "debuggingutils.h"

/*! \class InteractiveObject
        This class contains the definition of an object in the scene, (both the simulation and the animation data \n
        The animation data is containted in the _animation object, and the simulation object is defined by several objects _motion_state, _body etc)
    */
class Part
{
public:
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
        ANIMATION_Z,
        SIMULATION_X,
        SIMULATION_Y,
        SIMULATION_Z,
        ANIMATION_SPEED,
        SIMULATION_SPEED,
        NUMBER_OF_CURVES
    };
    Part();
    ~Part();

    //! calculates the information about the energy of the part of the body
    /*!
        calculates the information about the energy of the part of the body \n
        This is where the kinetic energy and speed and potential energy are calculated

    */
    void updatePartInfo(float elapsed, float diff, float gravity, const btTransform &transform, const btTransform &parent_transform);
    t_part_info getEnergyInformation() const {return _energy;}
    btScalar get_mass() const {return _mass;}
    void set_mass(btScalar mass){_mass = mass;}
    btScalar get_center_of_mass_proportion() const {return _center_of_mass_proportion;}
    void set_center_of_mass_proportion(btScalar center_of_mass_proportion){_center_of_mass_proportion = center_of_mass_proportion;}

    const btTransform& get_original_transform() const {return _original_transform;}
    void set_original_transform(const btTransform& transform){_original_transform = transform;}

    btRigidBody * get_body(){ return _simulation.get_body(); }

    bool is_animated() const {return _animated;}
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
    const Shape& get_shape_struct() const {return _shape;}

    void buildMesh();
    void buildMotion();
    state_information _animation_information;
    state_information _simulation_information;

    void transformStateMatch();
    void initialTransformStateMatch(btTransform node_transform, btTransform parent_transform);
    void updateStepsCurves(float elapsed){ insertDataToCurves(_curves_steps,elapsed);}
private:

    Part(const Part& object);
    void __build(const btVector3& origin, const btVector3& shape,Shape::shapetype type);
    void deleteMotion();

    MeshPointer _mesh;
    Shape _shape;

    btTransform _original_transform;

    QList<Curve> _curves; //! The curves recorded at each update of the status of objects
    QList<Curve> _curves_steps; //! The curves recorded at each step of the simulation

    QString _body_part_name; //! Name of the body part
    QString _parent_body_part_name; //! Name of the parent
    btScalar _mass;
    btScalar _center_of_mass_proportion;
    bool _animated;

    t_part_info _energy;

    void insertDataToCurves(QList<Curve>& curves, float elapsed);

    //! updates the structure that will be used for exportation
    void updateEnergyStructure(float gravity, float time);
    void setSimulationTransformFromAnimation();
    btScalar get_density(){
        return _mass / _shape.get_volume();
    }
    void updateSimulation(float delta_t);
    void updateAnimation(float delta_t, const btTransform &transform, const btTransform &parent_transform);

    SimulatedObject _simulation;

};

#endif // PART_H
