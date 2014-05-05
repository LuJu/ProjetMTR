#include "interactiveobject.h"

InteractiveObject::InteractiveObject(const btVector3& origin, const btVector3& shape,Shape::shapetype type):
    _mass(1),
    _center_of_mass_proportion(.5f),
    _animated(false){
    qDebug()<<"Not default constructor called";
    __build(origin,shape,type);
}

InteractiveObject::InteractiveObject():
    _mass(1),
    _center_of_mass_proportion(.5f),
    _animated(false){
    __build(btVector3(0,0,0),btVector3(1,1,1),Shape::cube);
}

void InteractiveObject::appendCurve(QList<Curve>& list, int index,QString label, QColor color){
    Curve& c= list[index];
    c.set_color(color);
    c.set_label(label);
}

InteractiveObject::~InteractiveObject(){
}

void InteractiveObject::__build(const btVector3& origin, const btVector3& shape,Shape::shapetype type){
    _shape = Shape(shape,type);
    _animation.set_shape(&_shape);
    _simulation.set_shape (&_shape);

    _original_transform.setIdentity();
    _original_transform.setOrigin(origin);

    appendCurves(_curves);
    appendCurves(_curves_steps);
}

void InteractiveObject::appendCurves(QList<Curve>& list){
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



void InteractiveObject::deleteMotion(){
}

void InteractiveObject::buildMotion(){
    _simulation.buildMotion(_mass,_original_transform,_center_of_mass_proportion);
}

void InteractiveObject::buildMesh(){
    _mesh = MeshPointer(new Mesh);
    _shape.set_shape(btVector3(_animation.scalingVector(0)));
    MeshUtils::addCapsuleShape(_mesh.data(),_shape.get_shape().y(),_shape.get_shape().x());
}

void InteractiveObject::updatePartInfo(float elapsed,float delta_t,float gravity,const btTransform& transform){

    _animation._information._previous = _animation._information._current;
    _simulation._information._previous = _simulation._information._current;

    updateAnimation(delta_t,transform);
    updateSimulation(delta_t);

    updateEnergyStructure(gravity);
    insertDataToCurves(_curves,elapsed);
}

void InteractiveObject::updateAnimation(float delta_t,const btTransform& transform){
    t_state_data& current = _animation._information._current;
    t_state_data& previous =_animation._information._previous;
    current._center_of_mass_world_position = transform.getOrigin() - _animation.centerToBaseVector();
    current._rotation = btQuat2euler(transform.getRotation());
    btVector3 animation_distance(current._center_of_mass_world_position - previous._center_of_mass_world_position);

    current._center_of_mass_world_speed = animation_distance/(delta_t/1000); // the diff value is in ms so a conversion is needed to be in m/s
    current._angular_speed = (current._rotation - previous._rotation) / (delta_t/1000) ;
    current._rotation_vector_diff = current._rotation -previous._rotation;
    if (current._rotation_vector_diff.length() != 0)
        current._rotation_vector_diff.normalize();
}

void InteractiveObject::updateSimulation(float delta_t){
    t_state_data& current = _simulation._information._current;
    t_state_data& previous =_simulation._information._previous;
    current._center_of_mass_world_position = get_body()->getCenterOfMassPosition();

    btVector3 simulation_distance = current._center_of_mass_world_position-previous._center_of_mass_world_position;
    btQuaternion rot = get_body()->getCenterOfMassTransform().getRotation();
    _calculated_simulation_speed = simulation_distance/(delta_t/1000);

    current._angular_speed= get_body()->getAngularVelocity();
    current._center_of_mass_world_speed = get_body()->getLinearVelocity();
    current._rotation = btQuat2euler(rot);
    current._rotation_vector_diff = current._rotation -previous._rotation;
    if (current._rotation_vector_diff.length() != 0)
        current._rotation_vector_diff.normalize();
}

void InteractiveObject::updateEnergyStructure(float gravity){

    _energy.part_name = _body_part_name;

    _energy.animation.position.x = _animation._information._current._center_of_mass_world_position.x();
    _energy.animation.position.y = _animation._information._current._center_of_mass_world_position.y();
    _energy.animation.position.z = _animation._information._current._center_of_mass_world_position.z();

    _energy.simulation.position.x = _simulation._information._current._center_of_mass_world_position.x();
    _energy.simulation.position.y = _simulation._information._current._center_of_mass_world_position.y();
    _energy.simulation.position.z = _simulation._information._current._center_of_mass_world_position.z();

    _energy.animation.speed = _animation._information._current._center_of_mass_world_speed.length();
    _energy.simulation.speed = (_simulation._information._current._center_of_mass_world_speed.length());

    _energy.animation.aspeed = _animation._information._current._angular_speed.length();
    _energy.animation.pt_aspeed.x = _animation._information._current._angular_speed.x();
    _energy.animation.pt_aspeed.y = _animation._information._current._angular_speed.y();
    _energy.animation.pt_aspeed.z = _animation._information._current._angular_speed.z();

    _energy.simulation.aspeed =      rad2deg(_simulation._information._current._angular_speed.length());
    _energy.simulation.pt_aspeed.x = rad2deg(_simulation._information._current._angular_speed.x());
    _energy.simulation.pt_aspeed.y = rad2deg(_simulation._information._current._angular_speed.y());
    _energy.simulation.pt_aspeed.z = rad2deg(_simulation._information._current._angular_speed.z());

    _energy.animation.ke  = translationKineticEnergy( _animation._information._current._center_of_mass_world_speed.length(),_mass);
    _energy.simulation.ke = translationKineticEnergy( _simulation._information._current._center_of_mass_world_speed.length(),_mass);

    _energy.animation.pe  = potentialEnergy(_mass,-gravity,_animation._information._current._center_of_mass_world_position.y());
    _energy.simulation.pe = potentialEnergy(_mass,-gravity,_simulation._information._current._center_of_mass_world_position.y());

    btVector3 aspeed_rad_animation, aspeed_rad_simulation;
    aspeed_rad_animation = btVector3(deg2rad(_animation._information._current._angular_speed.x()),
                                     deg2rad(_animation._information._current._angular_speed.y()),
                                     deg2rad(_animation._information._current._angular_speed.z()));
    aspeed_rad_simulation = _simulation._information._current._angular_speed;

    if (_animation._information._current._rotation_vector_diff.length() == 0){
        qDebug()<<"no a speed animation";
        _energy.animation.ake = 0;
    } else {
        _energy.animation.ake = angularKineticEnergy(aspeed_rad_animation,_animation._information._current._rotation_vector_diff,_shape.get_shape(),_mass);
    }
    if (_animation._information._current._rotation_vector_diff.length() == 0){
        qDebug()<<"no a speed simulation";
        _energy.simulation.ake = 0;
    } else {
        _energy.simulation.ake = angularKineticEnergy(aspeed_rad_simulation,_simulation._information._current._rotation_vector_diff,_shape.get_shape(),_mass);
    }

    _energy.ake_diff = _energy.animation.ake - _energy.simulation.ake;
    _energy.ke_diff =  _energy.animation.ke - _energy.simulation.ke;
    _energy.pe_diff =  _energy.animation.pe - _energy.simulation.pe;

}

void InteractiveObject::insertDataToCurves(QList<Curve>& curves, float elapsed){
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

void InteractiveObject::setSimulationTransformFromAnimation(){
    btVector3 translation,rotation;
    btQuaternion quat;
    btTransform transform;

    transform.setIdentity();
    rotation = _animation._information._current._rotation;
    quat.setEuler(deg2rad(rotation.y()),
                  deg2rad(rotation.x()),
                  deg2rad(rotation.z()));
    transform.setRotation(quat);

    translation = _animation._information._current._center_of_mass_world_position;
    transform.setOrigin(translation);
    _original_transform = transform;
}

void InteractiveObject::setSimulationPosition(float time){

    if (time != 0) insertDataToCurves(_curves_steps,time);

    setSimulationTransformFromAnimation();
    buildMotion();
    btRigidBody * body = get_body();
    if (time != 0) {
//        buildMotion();
        body->setLinearVelocity(_animation._information._current._center_of_mass_world_speed);
        body->setAngularVelocity(btVector3(deg2rad(_animation._information._current._angular_speed.x()),
                                          deg2rad(_animation._information._current._angular_speed.y()),
                                          deg2rad(_animation._information._current._angular_speed.z())));
    } else { // if the simulation is starting

//        _animation._information._current._center_of_mass_world_speed = speedAtTime(0.0f);
//        _animation._information._current._angular_speed = _animation.rotationTangent(time) ;
//        body->setLinearVelocity(_animation._information._current._center_of_mass_world_speed);
//        body->setAngularVelocity(btVector3(deg2rad(_animation._information._current._angular_speed.x()),
//                                          deg2rad(_animation._information._current._angular_speed.y()),
//                                          deg2rad(_animation._information._current._angular_speed.z())));
        _animation._information._previous._center_of_mass_world_position = _animation.extremityTranslationVector(0.0f);
        _simulation._information._previous._center_of_mass_world_position = body->getCenterOfMassPosition();
//        _simulation._information._previous._position_2 = _animation._information._previous._position;
        _simulation._information._previous._center_of_mass_world_speed = body->getLinearVelocity();
        _animation._information._previous._rotation = _animation.rotationVector(0.0f);
        _calculated_simulation_speed = _animation._information._current._center_of_mass_world_speed;
    }

    _simulation._information._previous._center_of_mass_world_speed = _animation._information._current._center_of_mass_world_speed; // sets the previous speed to the same as currens speed to avoid calculation errors
}

void InteractiveObject::setSimulationPosition2(btTransform transform, float time){

    insertDataToCurves(_curves_steps,time);// records the data for the curves
    t_state_data& current = _animation._information._current;
    current._rotation = btQuat2euler(transform.getRotation());
    current._center_of_mass_world_position = transform.getOrigin() - _animation.centerToBaseVector();

    setSimulationTransformFromAnimation();
    buildMotion();
    btRigidBody * body = get_body();
    body->setLinearVelocity(_animation._information._current._center_of_mass_world_speed);
    body->setAngularVelocity(btVector3(deg2rad(_animation._information._current._angular_speed.x()),
                                      deg2rad(_animation._information._current._angular_speed.y()),
                                      deg2rad(_animation._information._current._angular_speed.z())));
    _animation._information._previous._center_of_mass_world_position = _animation.extremityTranslationVector(0.0f);
    _simulation._information._previous._center_of_mass_world_position = body->getCenterOfMassPosition();
    _simulation._information._previous._center_of_mass_world_speed = body->getLinearVelocity();
    _animation._information._previous._rotation = _animation.rotationVector(0.0f);
    qDebug()<<toString(body->getAngularVelocity(),"ang velo: ");
    _calculated_simulation_speed = _animation._information._current._center_of_mass_world_speed;
}

void InteractiveObject::setInitialPosition(btTransform transform){

        t_state_data& current = _animation._information._current;
        current._rotation = btQuat2euler(transform.getRotation());
        current._center_of_mass_world_position = transform.getOrigin() - _animation.centerToBaseVector();

        setSimulationTransformFromAnimation();
        buildMotion();
        btRigidBody* body = get_body();
         // if the simulation is starting
        body->setLinearVelocity(_animation._information._current._center_of_mass_world_speed);
        _animation._information._previous._center_of_mass_world_position = _animation.extremityTranslationVector(0.0f);
        _simulation._information._previous._center_of_mass_world_position = body->getCenterOfMassPosition();
        _simulation._information._previous._center_of_mass_world_speed = body->getLinearVelocity();
        _animation._information._previous._rotation = _animation.rotationVector(0.0f);
        _calculated_simulation_speed = _animation._information._current._center_of_mass_world_speed;
}
