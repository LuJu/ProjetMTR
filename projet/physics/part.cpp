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
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/
#include "part.h"

Part::Part(const btVector3& origin, const btVector3& shape,Shape::shapetype type):
    _mass(1),
    _center_of_mass_proportion(.5f),
    _animated(false){
    __build(origin,shape,type);
}

Part::Part():
    _mass(1),
    _center_of_mass_proportion(.5f),
    _animated(false){
    __build(btVector3(0,0,0),btVector3(1,1,1),Shape::cube);
}

Part::~Part(){
}

void Part::__build(const btVector3& origin, const btVector3& shape,Shape::shapetype type){
    _shape = Shape(shape,type);

    _original_transform.setIdentity();
    _original_transform.setOrigin(origin);

    for (int i = 0; i < NUMBER_OF_CURVES; ++i) {
        _curves.append(Curve());
        _curves_steps.append(Curve());
    }
}

void Part::deleteMotion(){
}

void Part::buildMotion(){
    _simulation.buildMotion(_mass,_original_transform,_shape,_center_of_mass_proportion);
}

void Part::buildMesh(){
    _mesh = MeshPointer(new Mesh);
    MeshUtils::addCapsuleShapeY(_mesh.data(),_shape.get_shape().y(),_shape.get_shape().x());
    _mesh.data()->toBuffer();
}

void Part::updatePartInfo(float elapsed,float delta_t,float gravity,const btTransform& transform, const btTransform& parent_transform){

    _animation_information._previous = _animation_information._current;
    _simulation_information._previous = _simulation_information._current;

    updateAnimation(delta_t,transform,parent_transform);
    updateSimulation(delta_t);

    updateEnergyStructure(gravity,elapsed);
    insertDataToCurves(_curves,elapsed);
}

void Part::updateAnimation(float delta_t,const btTransform& transform, const btTransform& parent_transform){
    t_state_data& current = _animation_information._current;
    t_state_data& previous =_animation_information._previous;

    btScalar rotation_angle;


    btVector3 rotation_axis,parent_pos,child_pos;
    child_pos = transform.getOrigin();
    parent_pos = parent_transform.getOrigin();
    btVector3 vector_dist = - parent_pos + child_pos; // distance betwenn the two nodes
    btVector3 shape_vector = btVector3(0,_shape.get_shape().y()/2,0);
    btScalar dot_product = shape_vector.dot(vector_dist);
    btScalar norm_product = shape_vector.length() * vector_dist.length();
    if (absolute_value(dot_product-norm_product) < 0.00001){
        rotation_angle = 0;
        rotation_axis = btVector3(1,0,0);
    } else if (absolute_value(dot_product+norm_product)< 0.00001){
        rotation_angle = M_PI;
        rotation_axis = btVector3(1,0,0);
    } else {
        rotation_angle = angleDifference(shape_vector,vector_dist);
        rotation_axis = shape_vector.cross(vector_dist);
    }
    btQuaternion object_rotation(btVector3(rotation_axis),rotation_angle);
    btVector3 base_to_center = rotatedVector(object_rotation,shape_vector);

    current._center_of_mass_rotation = object_rotation;

    btQuaternion rotation_difference = previous._center_of_mass_rotation.inverse() * current._center_of_mass_rotation;

    current._center_of_mass_world_position = transform.getOrigin() - base_to_center;
    btVector3 animation_distance(current._center_of_mass_world_position - previous._center_of_mass_world_position);
    current._center_of_mass_world_speed = animation_distance/(delta_t/1000); // the diff value is in ms so a conversion is needed to be in m/s

    if(rotation_difference.w() < 0 ) rotation_difference = - rotation_difference;
        current._rotation_vector_diff = rotation_difference.getAxis();
        current._angular_speed = (rotation_difference.getAxis() * rotation_difference.getAngle())/(delta_t/1000);
    if (current._rotation_vector_diff.length() != 0)
        current._rotation_vector_diff.normalize();
}

void Part::updateSimulation(float delta_t){
    t_state_data& current = _simulation_information._current;
    t_state_data& previous =_simulation_information._previous;
    btRigidBody * body = get_body();

    current._center_of_mass_world_position = body->getCenterOfMassPosition();
    current._angular_speed=                  body->getAngularVelocity();
    current._center_of_mass_world_speed =    body->getLinearVelocity();
    current._center_of_mass_rotation =       body->getCenterOfMassTransform().getRotation();

    btVector3 simulation_distance = current._center_of_mass_world_position-previous._center_of_mass_world_position;

    btQuaternion rotation_difference = previous._center_of_mass_rotation.inverse() * current._center_of_mass_rotation;
    current._rotation_vector_diff = rotation_difference.getAxis();
    if (current._rotation_vector_diff.length() != 0)
        current._rotation_vector_diff.normalize();
}

void Part::updateEnergyStructure(float gravity,float time){

    _energy.part_name = _body_part_name;
    _energy.time = time;

    _energy.animation.position.x = _animation_information._current._center_of_mass_world_position.x();
    _energy.animation.position.y = _animation_information._current._center_of_mass_world_position.y();
    _energy.animation.position.z = _animation_information._current._center_of_mass_world_position.z();

    _energy.simulation.position.x = _simulation_information._current._center_of_mass_world_position.x();
    _energy.simulation.position.y = _simulation_information._current._center_of_mass_world_position.y();
    _energy.simulation.position.z = _simulation_information._current._center_of_mass_world_position.z();

    _energy.animation.speed = _animation_information._current._center_of_mass_world_speed.length();
    _energy.simulation.speed = (_simulation_information._current._center_of_mass_world_speed.length());

    _energy.animation.aspeed =      rad2deg(_animation_information._current._angular_speed.length());
    _energy.animation.pt_aspeed.x = rad2deg(_animation_information._current._angular_speed.x());
    _energy.animation.pt_aspeed.y = rad2deg(_animation_information._current._angular_speed.y());
    _energy.animation.pt_aspeed.z = rad2deg(_animation_information._current._angular_speed.z());

    _energy.simulation.aspeed =      rad2deg(_simulation_information._current._angular_speed.length());
    _energy.simulation.pt_aspeed.x = rad2deg(_simulation_information._current._angular_speed.x());
    _energy.simulation.pt_aspeed.y = rad2deg(_simulation_information._current._angular_speed.y());
    _energy.simulation.pt_aspeed.z = rad2deg(_simulation_information._current._angular_speed.z());

    _energy.animation.ke  = translationKineticEnergy( _animation_information._current._center_of_mass_world_speed.length(),_mass);
    _energy.simulation.ke = translationKineticEnergy( _simulation_information._current._center_of_mass_world_speed.length(),_mass);

    _energy.animation.pe  = potentialEnergy(_mass,-gravity,_animation_information._current._center_of_mass_world_position.y());
    _energy.simulation.pe = potentialEnergy(_mass,-gravity,_simulation_information._current._center_of_mass_world_position.y());

    btVector3 aspeed_rad_animation, aspeed_rad_simulation;
    aspeed_rad_animation = btVector3(_animation_information._current._angular_speed.x(),
                                     _animation_information._current._angular_speed.y(),
                                     _animation_information._current._angular_speed.z());
    aspeed_rad_simulation = _simulation_information._current._angular_speed;

    if (_animation_information._current._rotation_vector_diff.length() == 0) _energy.animation.ake = 0;
    else _energy.animation.ake = angularKineticEnergy(aspeed_rad_animation,_animation_information._current._rotation_vector_diff,_shape.get_shape(),_mass);
    if (_animation_information._current._rotation_vector_diff.length() == 0) _energy.simulation.ake = 0;
    else _energy.simulation.ake = angularKineticEnergy(aspeed_rad_simulation,_simulation_information._current._rotation_vector_diff,_shape.get_shape(),_mass);

    _energy.ake_diff = _energy.animation.ake - _energy.simulation.ake;
    _energy.ke_diff =  _energy.animation.ke - _energy.simulation.ke;
    _energy.pe_diff =  _energy.animation.pe - _energy.simulation.pe;

}

void Part::insertDataToCurves(QList<Curve>& curves, float elapsed){
    curves[ANIMATION_KE].insert(elapsed,_energy.animation.ke);
    curves[ANIMATION_AKE].insert(elapsed,_energy.animation.ake);
    curves[ANIMATION_PE].insert(elapsed,_energy.animation.pe);
    curves[SIMULATION_KE].insert(elapsed,_energy.simulation.ke);
    curves[SIMULATION_AKE].insert(elapsed,_energy.simulation.ake);
    curves[SIMULATION_PE].insert(elapsed,_energy.simulation.pe);
    curves[DIFF_KE].insert(elapsed,_energy.ke_diff);
    curves[DIFF_AKE].insert(elapsed,_energy.ake_diff);
    curves[DIFF_PE].insert(elapsed,_energy.pe_diff);
    curves[ANIMATION_X].insert(elapsed,_energy.animation.position.x);
    curves[ANIMATION_Y].insert(elapsed,_energy.animation.position.y);
    curves[ANIMATION_Z].insert(elapsed,_energy.animation.position.z);
    curves[SIMULATION_X].insert(elapsed,_energy.simulation.position.x);
    curves[SIMULATION_Y].insert(elapsed,_energy.simulation.position.y);
    curves[SIMULATION_Z].insert(elapsed,_energy.simulation.position.z);
    curves[ANIMATION_SPEED].insert(elapsed,_energy.animation.speed);
    curves[SIMULATION_SPEED].insert(elapsed,_energy.simulation.speed);
}

void Part::setSimulationTransformFromAnimation(){
    btVector3 translation;
    btTransform transform;

    transform.setIdentity();
    transform.setRotation( _animation_information._current._center_of_mass_rotation);
    translation = _animation_information._current._center_of_mass_world_position;
    transform.setOrigin(translation);

    _original_transform = transform;
}

void Part::transformStateMatch(){
    t_state_data& current_a = _animation_information._current;
    t_state_data& current_s = _simulation_information._current;
    t_state_data& previous_a = _animation_information._previous;
    t_state_data& previous_s = _simulation_information._previous;
    btRigidBody * body ;
    btTransform transform;

    transform.setIdentity();
    transform.setRotation( current_a._center_of_mass_rotation);
    transform.setOrigin(current_a._center_of_mass_world_position);
    _original_transform = transform;

    buildMotion();
    body = get_body();
    body->setLinearVelocity(current_a._center_of_mass_world_speed);
    body->setAngularVelocity(current_a._angular_speed);

    current_s._center_of_mass_world_position = body->getCenterOfMassPosition();
    previous_s._center_of_mass_world_position = previous_a._center_of_mass_world_position;

    current_s._center_of_mass_world_speed = body->getLinearVelocity();
    previous_s._center_of_mass_world_speed = previous_a._center_of_mass_world_speed;

    current_s._angular_speed = body->getAngularVelocity();
    previous_s._angular_speed =  previous_a._angular_speed;

    current_s._center_of_mass_rotation = transform.getRotation();
    previous_s._center_of_mass_rotation = previous_a._center_of_mass_rotation;
}

void Part::initialTransformStateMatch(btTransform node_transform,btTransform parent_transform){

        t_state_data& current_a = _animation_information._current;
        t_state_data& current_s = _simulation_information._current;
        t_state_data& previous_a = _animation_information._previous;
        t_state_data& previous_s = _simulation_information._previous;

        btScalar rotation_angle;
        btVector3 rotation_axis,parent_pos,child_pos;
        btRigidBody * body ;
        btTransform transform;

        child_pos = node_transform.getOrigin();
        parent_pos = parent_transform.getOrigin();

        btVector3 vector_dist = - parent_pos + child_pos; // distance betwenn the two nodes
        btVector3 shape_vector = btVector3(0,_shape.get_shape().y()/2,0);
        btScalar dot_product = shape_vector.dot(vector_dist);
        btScalar norm_product = shape_vector.length() * vector_dist.length();
        if (absolute_value(dot_product-norm_product) < 0.00001){// two vectors in the same direction
            rotation_angle = 0;
            rotation_axis = btVector3(1,0,0);
        } else if (absolute_value(dot_product+norm_product)< 0.00001){ // two vectors in the opposite direction
            rotation_angle = M_PI;
            rotation_axis = btVector3(1,0,0);
        } else {
            rotation_angle = angleDifference(shape_vector,vector_dist);
            rotation_axis = shape_vector.cross(vector_dist);
        }

        btQuaternion object_rotation(btVector3(rotation_axis),rotation_angle);
        btVector3 base_to_center = rotatedVector(object_rotation,shape_vector);

        current_a._center_of_mass_rotation = object_rotation;
        current_a._center_of_mass_world_position = node_transform.getOrigin() - base_to_center;
        current_a._center_of_mass_world_speed = btVector3(0,0,0);

        transform.setIdentity();
        transform.setRotation( current_a._center_of_mass_rotation);
        transform.setOrigin(current_a._center_of_mass_world_position);
        _original_transform = transform;

        buildMotion();
        body = get_body();

        current_s._center_of_mass_world_position = body->getCenterOfMassPosition();
        current_s._center_of_mass_world_speed = body->getLinearVelocity();
        current_s._angular_speed = current_a._angular_speed;
        current_s._center_of_mass_rotation = transform.getRotation();


        previous_s = current_s;
        previous_a = current_a;

}
