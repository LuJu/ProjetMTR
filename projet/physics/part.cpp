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
    qDebug()<<"Not default constructor called";
    __build(origin,shape,type);
}

Part::Part():
    _mass(1),
    _center_of_mass_proportion(.5f),
    _animated(false){
    __build(btVector3(0,0,0),btVector3(1,1,1),Shape::cube);
}

void Part::appendCurve(QList<Curve>& list, int index,QString label, QColor color){
    Curve& c= list[index];
    c.set_color(color);
    c.set_label(label);
}

Part::~Part(){
}

void Part::__build(const btVector3& origin, const btVector3& shape,Shape::shapetype type){
    _shape = Shape(shape,type);
    _simulation.set_shape (&_shape);

    _original_transform.setIdentity();
    _original_transform.setOrigin(origin);

    appendCurves(_curves);
    appendCurves(_curves_steps);
}

void Part::appendCurves(QList<Curve>& list){
    for (int i = 0; i < NUMBER_OF_CURVES; ++i) {
        list.append(Curve());
    }
    appendCurve(list,ANIMATION_KE,"Animation KE",QColor(255,0,0));
    appendCurve(list,ANIMATION_AKE,"Animation KE",QColor(0,0,255));
    appendCurve(list,ANIMATION_PE,"Animation KE",QColor(255,255,255));
    appendCurve(list,SIMULATION_KE,"Animation KE",QColor(255,0,0));
    appendCurve(list,SIMULATION_AKE,"Animation KE",QColor(0,0,255));
    appendCurve(list,SIMULATION_PE,"Animation KE",QColor(255,255,25));
    appendCurve(list,DIFF_KE,"Animation KE",QColor(255,0,255));
    appendCurve(list,DIFF_AKE,"Animation KE",QColor(0,255,255));
    appendCurve(list,DIFF_PE,"Animation KE",QColor(255,255,0));
    appendCurve(list,ANIMATION_X,"Animation KE",QColor(255,0,0));
    appendCurve(list,ANIMATION_Y,"Animation KE",QColor(255,0,255));
    appendCurve(list,SIMULATION_Y,"Animation KE",QColor(0,255,255));
}



void Part::deleteMotion(){
}

void Part::buildMotion(){
    _simulation.buildMotion(_mass,_original_transform,_center_of_mass_proportion);
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

    updateEnergyStructure(gravity);
    insertDataToCurves(_curves,elapsed);
}

btScalar Part::angleDifference(btVector3 v1, btVector3 v2){
    btScalar dot_product = v1.dot(v2);
    btScalar cost = dot_product / (v2.length() * v1.length());
    btScalar rotation_angle = qAcos(cost);
    return rotation_angle;
}

void Part::updateAnimation(float delta_t,const btTransform& transform, const btTransform& parent_transform){
    t_state_data& current = _animation_information._current;
    t_state_data& previous =_animation_information._previous;

    btScalar rotation_angle;
    btVector3 rotation_axis,parent_pos,child_pos;
    child_pos = transform.getOrigin();
    parent_pos = parent_transform.getOrigin();
    btVector3 vector_dist = - parent_pos + child_pos;
    btVector3 shape_vector = btVector3(0,_shape.get_shape().y()/2,0);
    rotation_angle = angleDifference(shape_vector,vector_dist);
//    rotation_angle = qAcos(cost);
    rotation_axis = shape_vector.cross(vector_dist);

    btQuaternion object_rotation(btVector3(rotation_axis),rotation_angle);
    btVector3 btc_bt = rotatedVector(object_rotation,shape_vector);

    current._center_of_mass_rotation = object_rotation;
    current._center_of_mass_world_position = transform.getOrigin() - btc_bt;
    btVector3 animation_distance(current._center_of_mass_world_position - previous._center_of_mass_world_position);
    current._center_of_mass_world_speed = animation_distance/(delta_t/1000); // the diff value is in ms so a conversion is needed to be in m/s

//    qDebug()<<toString(transform.getRotation(),"quat: ");
    btQuaternion slerp = current._center_of_mass_rotation.slerp(previous._center_of_mass_rotation,delta_t/1000);
    previous._center_of_mass_rotation = btQuaternion(0.3,0.4,0.1,0.7);
    btQuaternion conjugate(-previous._center_of_mass_rotation.x(),-previous._center_of_mass_rotation.y(),-previous._center_of_mass_rotation.z(),previous._center_of_mass_rotation.w());
    btQuaternion product( previous._center_of_mass_rotation * conjugate);
    btQuaternion derivated = slerp;
    btQuaternion distance(current._center_of_mass_rotation.inverse()*previous._center_of_mass_rotation);
    derivated *= 2;
    btQuaternion angular_velocity = derivated * conjugate;
//    qDebug()<<"deriv: "<<btQuat2euler(diff);
//    current._rotation_vector_diff = current._rotation -previous._rotation;
    if (current._rotation_vector_diff.length() != 0)
        current._rotation_vector_diff.normalize();
}

void Part::updateSimulation(float delta_t){
    t_state_data& current = _simulation_information._current;
    t_state_data& previous =_simulation_information._previous;
    btRigidBody * body = get_body();

//    qDebug()<<toString(body->getCenterOfMassPosition());

    current._center_of_mass_world_position = body->getCenterOfMassPosition();
    current._angular_speed=                  body->getAngularVelocity();
    current._center_of_mass_world_speed =    body->getLinearVelocity();
    current._center_of_mass_rotation =       body->getCenterOfMassTransform().getRotation();

    btVector3 simulation_distance = current._center_of_mass_world_position-previous._center_of_mass_world_position;
    _calculated_simulation_speed = simulation_distance/(delta_t/1000);

//    current._rotation = btQuat2euler(rot);
//    current._rotation_vector_diff = current._rotation -previous._rotation;
    if (current._rotation_vector_diff.length() != 0)
        current._rotation_vector_diff.normalize();
}

void Part::updateEnergyStructure(float gravity){

    _energy.part_name = _body_part_name;

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

    if (_animation_information._current._rotation_vector_diff.length() == 0){
//        qDebug()<<"no a speed animation";
        _energy.animation.ake = 0;
    } else {
        _energy.animation.ake = angularKineticEnergy(aspeed_rad_animation,_animation_information._current._rotation_vector_diff,_shape.get_shape(),_mass);
    }
    if (_animation_information._current._rotation_vector_diff.length() == 0){
//        qDebug()<<"no a speed simulation";
        _energy.simulation.ake = 0;
    } else {
        _energy.simulation.ake = angularKineticEnergy(aspeed_rad_simulation,_simulation_information._current._rotation_vector_diff,_shape.get_shape(),_mass);
    }

    _energy.ake_diff = _energy.animation.ake - _energy.simulation.ake;
    _energy.ke_diff =  _energy.animation.ke - _energy.simulation.ke;
    _energy.pe_diff =  _energy.animation.pe - _energy.simulation.pe;

}

void Part::insertDataToCurves(QList<Curve>& curves, float elapsed){
//        if (GlobalConfig::is_enabled("display_animation_stats")) {
//            curves[ANIMATION_KE].insert(elapsed,_energy.animation.ke);
//            curves[ANIMATION_AKE].insert(elapsed,_energy.animation.ake);
//            curves[ANIMATION_PE].insert(elapsed,_energy.animation.pe);
//        }
//        if (GlobalConfig::is_enabled("display_simulation_stats")) {
//            curves[SIMULATION_KE].insert(elapsed,_energy.simulation.ke);
//            curves[SIMULATION_AKE].insert(elapsed,_energy.simulation.ake);
//            curves[SIMULATION_PE].insert(elapsed,_energy.simulation.pe);
//        }
//        if (GlobalConfig::is_enabled("display_diff")) {
            curves[DIFF_KE].insert(elapsed,_energy.ke_diff);
            curves[DIFF_AKE].insert(elapsed,_energy.ake_diff);
            curves[DIFF_PE].insert(elapsed,_energy.pe_diff);
//            curves[ANIMATION_Y].insert(elapsed,_energy.animation.y);
//            curves[ANIMATION_X].insert(elapsed,_energy.animation.x);
//            curves[ANIMATION_Y].insert(elapsed,_energy.animation.y);
//        }
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

void Part::setSimulationPosition(btTransform transform, float time){

    insertDataToCurves(_curves_steps,time);// records the data for the curves
    t_state_data& current = _animation_information._current;
//    current._rotation = btQuat2euler(transform.getRotation());
//    if (current._rotation.y() > M_PI-0.01 && current._rotation.z() > M_PI-0.01){
//        current._rotation.setY(0);
//        current._rotation.setZ(0);
//    }
//    qDebug()<<toString(current._rotation,"rotation : ");
//    current._center_of_mass_world_position = transform.getOrigin() - rotatedVector(current._rotation,btVector3(_shape.get_shape().y()/2,0,0));
//    current._center_of_mass_world_position = transform.getOrigin() + rotatedVector(current._rotation,btVector3(0,-_shape.get_shape().y()/2,0));
//    current._center_of_mass_world_position = transform.getOrigin() - _animation.centerToBaseVector();

    setSimulationTransformFromAnimation();
    buildMotion();
    btRigidBody * body = get_body();
    body->setLinearVelocity(_animation_information._current._center_of_mass_world_speed);
//    body->setAngularVelocity(_animation_information._current._angular_speed);
//    body->setAngularVelocity(_animation_information._current._angular_speed);
//    qDebug()<<toString(rad2deg(_animation_information._current._angular_speed),"angular speed : ");

//    _animation_information._previous._center_of_mass_world_position = _animation.extremityTranslationVector(0.0f);
    _simulation_information._current._center_of_mass_world_position = body->getCenterOfMassPosition();
    _simulation_information._previous._center_of_mass_world_position = _animation_information._previous._center_of_mass_world_position;

    _simulation_information._current._center_of_mass_world_speed = body->getLinearVelocity();
    _simulation_information._previous._center_of_mass_world_speed = _animation_information._previous._center_of_mass_world_speed;

//    _simulation_information._current._rotation = _animation_information._current._rotation;
//    _simulation_information._previous._rotation = _animation_information._previous._rotation;

//    _simulation_information._current._angular_speed = body->getAngularVelocity();
//    _simulation_information._previous._angular_speed =  _animation_information._previous._angular_speed;

//    _animation_information._previous._rotation = _animation.rotationVector(0.0f);

    _calculated_simulation_speed = _animation_information._current._center_of_mass_world_speed;
}

void Part::setInitialPosition(btTransform transform,btTransform parent_transform){

        t_state_data& current = _animation_information._current;

//        current._center_of_mass_rotation= transform.getRotation();
//        current._rotation = btQuat2euler(transform.getRotation());
//        current._center_of_mass_world_position = transform.getOrigin() - rotatedVector(current._rotation,btVector3(_shape.get_shape().y()/2,0,0));

        btScalar rotation_angle;
        btVector3 rotation_axis,parent_pos,child_pos;
        child_pos = transform.getOrigin();
        parent_pos = parent_transform.getOrigin();
        btVector3 vector_dist = - parent_pos + child_pos;
        btVector3 shape_vector = btVector3(0,_shape.get_shape().y()/2,0);
        rotation_angle = angleDifference(shape_vector,vector_dist);
    //    rotation_angle = qAcos(cost);
        rotation_axis = shape_vector.cross(vector_dist);
        btQuaternion object_rotation(btVector3(rotation_axis),rotation_angle);
        btVector3 btc_bt = rotatedVector(object_rotation,shape_vector);

        current._center_of_mass_rotation = object_rotation;
        current._center_of_mass_world_position = transform.getOrigin() - btc_bt;
//        btVector3 animation_distance(current._center_of_mass_world_position - previous._center_of_mass_world_position);
//        current._center_of_mass_world_speed = animation_distance/(delta_t/1000); // the diff value is in ms so a conversion is needed to be in m/s




        setSimulationTransformFromAnimation();
        buildMotion();
        btRigidBody* body = get_body();
        body->setLinearVelocity(_animation_information._current._center_of_mass_world_speed);

//        _animation_information._previous._center_of_mass_world_position = _animation.extremityTranslationVector(0.0f);
        _simulation_information._previous._center_of_mass_world_position = body->getCenterOfMassPosition();
        _simulation_information._previous._center_of_mass_world_speed = body->getLinearVelocity();

//        _animation_information._previous._rotation = _animation.rotationVector(0.0f);

        _calculated_simulation_speed = _animation_information._current._center_of_mass_world_speed;
}
